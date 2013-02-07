#! /usr/bin/env python

import glob
import io
import os
import os.path
import ntpath
import stat
import uuid

# Different types of projects found within the solution.
LIBRARY_PROJECT = 0
TEST_PROJECT = 1
BENCHMARK_PROJECT = 2
SAMPLE_PROJECT = 3

project_groups = {
    BENCHMARK_PROJECT: 'benchmarks',
    LIBRARY_PROJECT: 'libraries',
    SAMPLE_PROJECT: 'samples',
    TEST_PROJECT: 'tests',
    }

# List of projects to generate within the solution. Format for each entry is:
#
# <project name> : (<project type>, <project root directory>, [<source directory>], [<project dependency>])
#
#       <project name> = name of the project, used for generating project file name
#       <project type> = constant specifying type of project
#       <project root directory> = root directory of source files, relative to $(UpcasteRoot)/src/
#       <source directory> = name of subdirectory relative to project root directory of source files to add to project
#       <project dependency> = name of projects that this project is dependent on
#       <3rd party dependency> = name of third-party dependencies to link against

projects = {
    'glew': (LIBRARY_PROJECT, 'glew', ['src'], []),
    'upcore': (LIBRARY_PROJECT, 'upcore', ['gen', 'src'], []),
    'upsystem': (LIBRARY_PROJECT, 'upsystem', ['src'], ['glew', 'upcore']),
    'uptest': (LIBRARY_PROJECT, 'uptest', ['src'], ['upcore']),
    'upcore_tests': (TEST_PROJECT, 'upcore', ['test'], ['upcore', 'uptest']),
    'upcore_bench': (BENCHMARK_PROJECT, 'upcore', ['bench'], ['upcore', 'uptest']),
    'upsystem_bench': (BENCHMARK_PROJECT, 'upsystem', ['bench'], ['upcore', 'upsystem', 'uptest']),
    'raycast': (SAMPLE_PROJECT, 'raycast', ['data', 'src'], ['glew', 'upcore', 'upsystem'])
    }

# Extra dependencies for applicable projects, each dependecy delimited by a semi-colon.
#
# <project name> : (<include directories>, <library dependencies>)

project_dependencies = {
    'glew': ('', 'OpenGL32.lib;'),
    'upsystem': ('opencl', 'OpenCL.lib;winmm.lib;')
    }

# Common directory paths.
top_dir = os.path.abspath(os.path.join(os.pardir, os.pardir))
include_dir = os.path.join(top_dir, 'include')
project_dir = os.path.join(top_dir, 'project')
sample_root_dir = os.path.join(top_dir, 'sample')
src_root_dir = os.path.join(top_dir, 'src')
solution_dir = os.path.join(project_dir, 'msvs11')

# Files and directories found under directories with the following
# names are added to projects but are ignored for compilation.
common_ignore_dirs = ['clang', 'gas', 'gcc', 'linux', 'macosx', 'ios', 'posix']

# Orthogonal variant types for each project configuration.
architectures = ['Win32', 'x64']
configurations = ['Debug', 'Release', 'Silver', 'Gold', 'Platinum']
library_linkage = ['Shared', 'Static']

###---==============================================================---
###                          MISCELLANEOUS
###---==============================================================---

def get_project_filepath(project_name, project_info):
    return os.path.join(project_groups[project_info[0]], project_name + '.vcxproj')

def get_sorted_directory_items(dirpath):
    dirs = list()
    files = list()
    if not os.path.isdir(dirpath):
        return (dirs, files)
    for item in os.listdir(dirpath):
        itempath = os.path.join(dirpath, item)
        if os.path.isfile(itempath) and not os.path.islink(itempath):
            files.append(itempath)
        elif os.path.isdir(itempath):
            dirs.append(itempath)
    return (sorted(dirs), sorted(files))

def get_sorted_subdirectories(dirpath):
    dirs = list()
    if not os.path.isdir(dirpath):
        return dirs
    for item in os.listdir(dirpath):
        itempath = os.path.join(dirpath, item)
        if os.path.isdir(itempath):
            dirs.append(itempath)
    return sorted(dirs)

def get_project_include_dirs(name, info):
    dirs = ''
    for dependency in info[3]:
        if len(dirs) > 0:
            dirs += ';'
        dirs += '$(UpcasteRootDir)include\\' + dependency
        if dependency in project_dependencies:
            dirs += ';$(UpcasteRootDir)include\\' + project_dependencies[dependency][0]
    if info[0] == LIBRARY_PROJECT:
        local_dir = '$(UpcasteRootDir)include\\' + name
        if len(dirs) > 0:
            dirs = local_dir + ';' + dirs
        else:
            dirs = local_dir
    if name in project_dependencies:
        dirs += ';$(UpcasteRootDir)include\\' + project_dependencies[name][0]
    return dirs

def get_project_libs(name, info):
    libs = ''
    for dependency in info[3]:
        if projects[dependency][0] == LIBRARY_PROJECT:
            libs += dependency + '.lib;'
        if dependency in project_dependencies:
            libs += project_dependencies[dependency][1]
    if name in project_dependencies:
        libs += project_dependencies[name][1]
    return libs

def get_source_root_dir(info):
    if info[0] == SAMPLE_PROJECT:
        return os.path.join(sample_root_dir, info[1])
    else:
        return os.path.join(src_root_dir, info[1])

def get_project_defs(name, info):
    def_files = dict()
    vcxproj_dir = os.path.join(solution_dir, project_groups[info[0]])
    project_src_root_dir = get_source_root_dir(info)
    for src_dir in info[2]:
        src_dir = os.path.join(project_src_root_dir, src_dir)
        for item in os.listdir(src_dir):
            itempath = os.path.join(src_dir, item)
            if os.path.isfile(itempath) and not os.path.islink(itempath):
                root, ext = os.path.splitext(itempath)
                if (ext == '.def'):
                    relinputfilepath = ntpath.normpath(os.path.relpath(itempath, vcxproj_dir))
                    _, def_tag = os.path.splitext(root.lower())
                    if def_tag == '.x64':
                        def_files['x64'] = relinputfilepath
                    elif def_tag == '.x86':
                        def_files['Win32'] = relinputfilepath
    return def_files


###---==============================================================---
###                 MSVS2010 PROJECT FILE GENERATION
###---==============================================================---

def build_project_files(project_uuids):
    for project_name, project_info in projects.items():
        build_project_file(project_name, project_uuids[project_name], project_info)

def build_project_file(project_name, project_uuid, project_info):
    project_filepath = get_project_filepath(project_name, project_info)

    # generate *.vcxproj file
    with open(project_filepath, 'wt') as output:
        output.write('<?xml version=\"1.0\" encoding=\"utf-8\"?>\n')
        output.write('<Project DefaultTargets=\"Build\" ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n')
        build_project_configuration_itemgroup(output)
        build_project_globals(output, project_name, project_uuid)
        output.write('  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.Default.props\" />\n')
        build_project_configuration_propertygroup(output, project_info)
        output.write('  <Import Project=\"$(VCTargetsPath)\\Microsoft.Cpp.props\" />\n')
        build_project_propertysheet_imports(output, project_info)
        output.write('  <PropertyGroup Label=\"UserMacros\" />\n')
        build_project_item_definition_groups(output, project_name, project_info)
        build_project_items(output, project_info)
        output.write('  <Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.targets\" />\n')
        build_project_extension_targets(output)
        output.write('</Project>\n')

    # generate *.vcxproj.filters file
    with open(project_filepath + '.filters', 'wt') as output:
        output.write('<?xml version="1.0" encoding="utf-8"?>\n')
        output.write('<Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">\n')
        build_project_directory_filters(output, project_info)
        build_project_file_filters(output, project_info)
        output.write('</Project>\n')

def build_project_globals(output, project_name, project_uuid):
    output.write('  <PropertyGroup Label=\"Globals\">\n')
    output.write('    <ProjectGuid>{{{0}}}</ProjectGuid>\n'.format(project_uuid))
    output.write('    <Keyword>Win32Proj</Keyword>\n')
    output.write('    <RootNamespace>{0}</RootNamespace>\n'.format(project_name))
    output.write('    <VCTargetsPath>$(VCTargetsPath11)</VCTargetsPath>\n')
    output.write('  </PropertyGroup>\n')

def build_project_configuration_itemgroup(output):
    output.write('  <ItemGroup Label=\"ProjectConfigurations\">\n')
    for linkage in library_linkage:
        for config in configurations:
            for arch in architectures:
                output.write('    <ProjectConfiguration Include=\"{0}-{1}|{2}\">\n'.format(linkage, config, arch))
                output.write('      <Configuration>{0}-{1}</Configuration>\n'.format(linkage, config))
                output.write('      <Platform>{0}</Platform>\n'.format(arch))
                output.write('    </ProjectConfiguration>\n')
    output.write('  </ItemGroup>\n')

def build_project_configuration_propertygroup(output, project_info):
    for linkage in library_linkage:
        for config in configurations:
            output.write('  <PropertyGroup Condition=\"\'$(Configuration)\'==\'{0}-{1}\'\" Label=\"Configuration\">\n'.format(linkage, config))
            output.write('    <CharacterSet>Unicode</CharacterSet>\n')
            if (project_info[0] == LIBRARY_PROJECT) and (linkage == 'Shared'):
                output.write('    <ConfigurationType>DynamicLibrary</ConfigurationType>\n')
            elif (project_info[0] == LIBRARY_PROJECT) and (linkage == 'Static'):
                output.write('    <ConfigurationType>StaticLibrary</ConfigurationType>\n')
            else:
                output.write('    <ConfigurationType>Application</ConfigurationType>\n')
            output.write('    <LinkIncremental>{0}</LinkIncremental>\n'.format('true' if config == 'Debug' else 'false'))
            output.write('    <PlatformToolset>v110</PlatformToolset>\n')
            output.write('    <UseDebugLibraries>{0}</UseDebugLibraries>\n'.format('true' if config == 'Debug' else 'false'))
            output.write('    <WholeProgramOptimization>{0}</WholeProgramOptimization>\n'.format('false' if config == 'Debug' else 'true'))
            output.write('  </PropertyGroup>\n')

def build_project_extension_settings(output):
    output.write('  <ImportGroup Label=\"ExtensionSettings\">\n')
    output.write('  </ImportGroup>\n')

def build_project_extension_targets(output):
    output.write('  <ImportGroup Label=\"ExtensionTargets\">\n')
    output.write('  </ImportGroup>\n')

def build_project_propertysheet_imports(output, project_info):
    for linkage in library_linkage:
        for config in configurations:
            output.write('  <ImportGroup Condition=\"\'$(Configuration)\'==\'{0}-{1}\'\" Label=\"PropertySheets\">\n'.format(linkage, config))
            output.write('    <Import Project=\"$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists(\'$(UserRootDir)\\Microsoft.Cpp.$(Platform).user.props\')\" Label=\"LocalAppDataPlatform\" />\n')
            output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Common.props\" />\n')
            output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Configuration.{0}.props" />\n'.format(config))
            output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Linkage.{0}.props\" />\n'.format(linkage))
            output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Platform.$(Platform).props\" />\n')
            if (project_info[0] == SAMPLE_PROJECT):
                output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Target.Application.props\" />\n')
            elif (project_info[0] == BENCHMARK_PROJECT) or (project_info[0] == TEST_PROJECT):
                output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Target.Console.props\" />\n')
            elif (project_info[0] == LIBRARY_PROJECT) and (linkage == 'Shared'):
                output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Target.DynamicLibrary.props\" />\n')
            elif (project_info[0] == LIBRARY_PROJECT) and (linkage == 'Static'):
                output.write('    <Import Project=\"$(SolutionDir)\\Upcaste.Cpp.Target.StaticLibrary.props\" />\n')
            output.write('  </ImportGroup>\n')

def build_project_item_definition_groups(output, project_name, project_info):
    # Determine additional dependency information
    lib_files = get_project_libs(project_name, project_info)
    def_files = get_project_defs(project_name, project_info)

    # Build item definition groups for each linkage type (static, shared)
    for linkage in library_linkage:
        for config in configurations:
            output.write('  <ItemDefinitionGroup Condition=\"\'$(Configuration)\'==\'{0}-{1}\'\">\n'.format(linkage, config))
            output.write('    <ClCompile>\n')
            output.write('      <AdditionalIncludeDirectories>{0}</AdditionalIncludeDirectories>\n'.format(get_project_include_dirs(project_name, project_info)))
            output.write('      <PrecompiledHeader />\n')
            preproc = ''
            if (linkage == 'Shared') and (project_info[0] == LIBRARY_PROJECT):
                preproc = 'LIB' + project_name.replace('_', '').upper() + '_EXPORT_SYMBOLS'
            if len(preproc) > 0:
                output.write('      <PreprocessorDefinitions>{0};%(PreprocessorDefinitions)</PreprocessorDefinitions>\n'.format(preproc))
            output.write('    </ClCompile>\n')
            if ((project_info[0] == LIBRARY_PROJECT) and (linkage == 'Static')) or ((len(lib_files) == 0) and (len(def_files) == 0)):
                output.write('    <Link />\n')
            else:
                output.write('    <Link>\n')
                if len(lib_files) > 0:
                        output.write('      <AdditionalDependencies>{0}%(AdditionalDependencies)</AdditionalDependencies>\n'.format(lib_files))
                if len(def_files) > 0:
                    for (platform, filepath) in def_files.iteritems():
                        output.write('      <ModuleDefinitionFile Condition=\"\'$(Platform)\'==\'{0}\'\">{1}</ModuleDefinitionFile>'.format(platform, filepath))
                output.write('    </Link>\n')
            # cludge to insert prebuild step in upsystem project so that OpenCL libraries are copied to the respective bin/lib folders
            if project_name == 'upsystem':
                output.write('    <PreBuildEvent>\n')
                output.write('        <Command>')
                output.write('copy $(UpcasteRootDir)depend\\bin\\$(UpcastePlatform)\\OpenCL.dll $(OutDir)OpenCL.dll\r\n')
                output.write('copy $(UpcasteRootDir)depend\\lib\\$(UpcastePlatform)\\OpenCL.lib $(UpcasteRootDir)lib\\$(UpcasteVariant)\\OpenCL.lib\r\n')
                output.write('        </Command>\n')
                output.write('    </PreBuildEvent>\n')
            output.write('  </ItemDefinitionGroup>\n')

def build_project_items(output, project_info):
    output.write('  <ItemGroup>\n')
    if (project_info[0] == TEST_PROJECT) or (project_info[0] == BENCHMARK_PROJECT):
        output.write('    <ClCompile Include=\"..\\..\\..\\src\\uptest\\driver\\console_driver.cpp\" />\n')
    vcxproj_dir = os.path.join(solution_dir, project_groups[project_info[0]])
    project_src_root_dir = get_source_root_dir(project_info)
    for src_dir in project_info[2]:
        build_project_items_recursive(output, os.path.join(project_src_root_dir, src_dir), vcxproj_dir, project_src_root_dir, False)
    output.write('  </ItemGroup>\n')

def build_project_items_recursive(output, dirpath, vcxproj_dir, project_src_root_dir, ignore_for_compilation):
    dirs, files = get_sorted_directory_items(dirpath)
    for filepath in files:
        relinputfilepath = ntpath.normpath(os.path.relpath(filepath, vcxproj_dir))
        reloutputpath = ntpath.normpath(os.path.relpath(dirpath, project_src_root_dir))
        if len(reloutputpath) > 0:
            reloutputpath = reloutputpath + '\\'
        root, ext = os.path.splitext(filepath)
        if not ignore_for_compilation:
            ext = ext.lower()
            if (ext == '.c') or (ext == '.cc') or (ext == '.cpp') or (ext == '.cxx'):
                # C/C++ source file
                output.write('    <ClCompile Include=\"{0}\" >\n'.format(relinputfilepath))
                output.write('        <AssemblerListingLocation>$(IntDir){0}%(FileName).asm</AssemblerListingLocation>\n'.format(reloutputpath))
                output.write('        <ObjectFileName>$(IntDir){0}</ObjectFileName>\n'.format(reloutputpath))
                output.write('    </ClCompile>\n')
                continue
            elif (ext == '.asm') or (ext == '.masm'):
                # MASM assembly file, get the tag in the filename to determine target platform
                asm_platform = None
                _, asm_tag = os.path.splitext(root.lower())
                if asm_tag == '.x64':
                    asm_ml = 'ML64'
                    asm_options = '/c /W3 /WX /Sa'
                    asm_platform = 'x64'
                elif asm_tag == '.x86':
                    asm_ml = 'ML'
                    asm_options = '/c /W3 /WX /Sa /safeseh'
                    asm_platform = 'Win32'
                if asm_platform != None:
                    # generate custom build step
                    asm_command = '@ECHO OFF\r\nIF NOT EXIST "$(IntDir){0}" MKDIR "$(IntDir){0}"\r\n'.format(reloutputpath)
                    asm_command += '{0} {1} \"/Fl$(IntDir){2}%(FileName).lst\" \"/Fo$(IntDir){2}%(FileName).obj\" \"%(FullPath)\"'.format(asm_ml, asm_options, reloutputpath)
                    output.write('    <CustomBuild Include=\"{0}\">\n'.format(relinputfilepath))
                    output.write('      <Command Condition=\"\'$(Platform)\'==\'{0}\'\">{1}</Command>\n'.format(asm_platform, asm_command))
                    output.write('      <Outputs Condition=\"\'$(Platform)\'==\'{0}\'\">$(IntDir){1}%(FileName).obj;%(Outputs)</Outputs>\n'.format(asm_platform, reloutputpath))
                    output.write('      <FileType>Document</FileType>\n')
                    output.write('    </CustomBuild>\n')
                    continue
        output.write('    <None Include=\"{0}\" />\n'.format(relinputfilepath))
    for subdirpath in dirs:
        build_project_items_recursive(output, subdirpath, vcxproj_dir, project_src_root_dir, (os.path.basename(subdirpath) in common_ignore_dirs) or ignore_for_compilation)

def build_project_directory_filters(output, project_info):
    output.write('  <ItemGroup>\n')
    if (project_info[0] == TEST_PROJECT) or (project_info[0] == BENCHMARK_PROJECT):
        output.write('    <Filter Include=\"driver\">\n')
        output.write('      <UniqueIdentifier>{{{0}}}</UniqueIdentifier>\n'.format(uuid.uuid1()))
        output.write('    </Filter>\n')
    root_dir = get_source_root_dir(project_info)
    for src_dir in project_info[2]:
        build_project_directory_filters_recursive(output, os.path.join(root_dir, src_dir), root_dir)
    output.write('  </ItemGroup>\n')

def build_project_directory_filters_recursive(output, dirpath, root_dir):
    output.write('    <Filter Include=\"{0}\">\n'.format(ntpath.normpath(os.path.relpath(dirpath, root_dir))))
    output.write('      <UniqueIdentifier>{{{0}}}</UniqueIdentifier>\n'.format(uuid.uuid1()))
    output.write('    </Filter>\n')
    for subdir in get_sorted_subdirectories(dirpath):
        build_project_directory_filters_recursive(output, subdir, root_dir)

def build_project_file_filters(output, project_info):
    output.write('  <ItemGroup>\n')
    if (project_info[0] == TEST_PROJECT) or (project_info[0] == BENCHMARK_PROJECT):
        output.write('    <ClCompile Include=\"..\\..\\..\\src\\uptest\\driver\\console_driver.cpp\">\n')
        output.write('      <Filter>driver</Filter>\n')
        output.write('    </ClCompile>\n')
    vcxproj_dir = os.path.join(solution_dir, project_groups[project_info[0]])
    root_dir = get_source_root_dir(project_info)
    for src_dir in project_info[2]:
        build_project_file_filters_recursive(output, os.path.join(root_dir, src_dir), root_dir, vcxproj_dir, False)
    output.write('  </ItemGroup>\n')

def build_project_file_filters_recursive(output, dirpath, root_dir, vcxproj_dir, ignore_for_compilation):
    dirs, files = get_sorted_directory_items(dirpath)
    for filepath in files:
        filter_type = 'None'
        relfilepath = os.path.relpath(filepath, vcxproj_dir)
        root, ext = os.path.splitext(filepath)
        if not ignore_for_compilation:
            ext = ext.lower()
            if (ext == '.c') or (ext == '.cc') or (ext == '.cpp') or (ext == '.cxx'):
                filter_type = 'ClCompile'
            elif (ext == '.asm') or (ext == '.masm'):
                filter_type = 'CustomBuild'
        output.write('    <{0} Include=\"{1}\">\n'.format(filter_type, ntpath.normpath(relfilepath)))
        output.write('      <Filter>{0}</Filter>\n'.format(ntpath.normpath(os.path.relpath(dirpath, root_dir))))
        output.write('    </{0}>\n'.format(filter_type))
    for subdirpath in dirs:
        build_project_file_filters_recursive(output, subdirpath, root_dir, vcxproj_dir, (os.path.basename(subdirpath) in common_ignore_dirs) or ignore_for_compilation)

###---==============================================================---
###                 MSVS2012 SOLUTION FILE GENERATION
###---==============================================================---

cxx_project_uuid = '8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942'
solution_folder_uuid = '2150E333-8FDC-42A3-9474-1A3956D46DE8'

def build_solution_file():
    project_uuids = dict()
    nested_project_uuids = list()
    with open(os.path.join(solution_dir, 'Upcaste.sln'), "wt") as output:
        output.write('Microsoft Visual Studio Solution File, Format Version 12.00\n')
        output.write('# Visual Studio 1112.00\n')
        build_solution_folder(output, include_dir, nested_project_uuids)
        build_solution_projects(output, project_uuids, nested_project_uuids)
        output.write('Global\n')
        output.write('\tGlobalSection(SolutionProperties) = preSolution\n')
        output.write('\t\tHideSolutionNode = FALSE\n')
        output.write('\tEndGlobalSection\n')
        build_solution_configuration_platforms(output)
        build_solution_project_configuration_platforms(output, project_uuids)
        build_solution_nested_projects(output, nested_project_uuids)
        output.write('EndGlobal\n')
    return project_uuids

def build_solution_folder(output, dirpath, nested_project_uuids):
    # get directories and files for folder
    dirs, files = get_sorted_directory_items(dirpath)

    # create solution folde for 'dirpath'
    folder_uuid = uuid.uuid1()
    output.write('Project(\"{{{0}}}\") = \"{1}\", \"{1}\", \"{{{2}}}\"\n'.format(solution_folder_uuid, ntpath.normpath(os.path.basename(dirpath)), folder_uuid))

    # add any files to the solution folder
    if len(files) >= 1:
        output.write('\tProjectSection(SolutionItems) = preProject\n')
        for file in files:
            output.write('\t\t{0} = {0}\n'.format(os.path.relpath(file, solution_dir)))
        output.write('\tEndProjectSection\n')

    output.write('EndProject\n')

    # recursively add sub-directories as nested solution folders
    if len(dirs) >= 1:
        for dir in dirs:
            sub_folder_uuid = build_solution_folder(output, dir, nested_project_uuids)
            nested_project_uuids.append((sub_folder_uuid, folder_uuid))

    return folder_uuid

def build_solution_projects(output, project_uuids, nested_project_uuids):
    # create root src folder
    src_folder_uuid = uuid.uuid1()
    output.write('Project(\"{{{0}}}\") = \"{1}\", \"{1}\", \"{{{2}}}\"\n'.format(solution_folder_uuid, 'src', src_folder_uuid))
    output.write('EndProject\n')

    # build project group folders for libraries, tests, benchmarks, etc.
    project_group_uuids = dict()
    for project_type, sub_folder_name in project_groups.items():
        sub_folder_uuid = uuid.uuid1()
        nested_project_uuids.append((sub_folder_uuid, src_folder_uuid))
        project_group_uuids[project_type] = sub_folder_uuid
        output.write('Project(\"{{{0}}}\") = \"{1}\", \"{1}\", \"{{{2}}}\"\n'.format(solution_folder_uuid, sub_folder_name, sub_folder_uuid))
        output.write('EndProject\n')

    # build project entries, nesting them within the project group folders
    for project_name in projects:
        project_uuids[project_name] = uuid.uuid1()

    for project_name, project_info in projects.items():
        project_uuid = project_uuids[project_name]
        nested_project_uuids.append((project_uuid, project_group_uuids[project_info[0]]))
        project_filepath = get_project_filepath(project_name, project_info)
        output.write('Project(\"{{{0}}}\") = \"{1}\", \"{2}\", \"{{{3}}}\"\n'.format(cxx_project_uuid, project_name, ntpath.normpath(project_filepath), project_uuid))
        if len(project_info[3]) >= 1:
            output.write('\tProjectSection(ProjectDependencies) = postProject\n')
            for project_dependency in project_info[3]:
                output.write('\t\t{{{0}}} = {{{0}}}\n'.format(project_uuids[project_dependency]))
            output.write('\tEndProjectSection\n')
        output.write('EndProject\n')

def build_solution_configuration_platforms(output):
    output.write('\tGlobalSection(SolutionConfigurationPlatforms) = preSolution\n')
    for linkage in library_linkage:
        for config in configurations:
            for arch in architectures:
                output.write('\t\t{0}-{1}|{2} = {0}-{1}|{2}\n'.format(linkage, config, arch))
    output.write('\tEndGlobalSection\n')

def build_solution_project_configuration_platforms(output, project_uuids):
    if len(project_uuids) >= 1:
        output.write('\tGlobalSection(ProjectConfigurationPlatforms) = postSolution\n')
        for project_name, project_uuid in project_uuids.items():
            for linkage in library_linkage:
                for config in configurations:
                    for arch in architectures:
                        output.write('\t\t{{{0}}}.{1}-{2}|{3}.ActiveCfg = {1}-{2}|{3}\n'.format(project_uuid, linkage, config, arch))
                        output.write('\t\t{{{0}}}.{1}-{2}|{3}.Build.0 = {1}-{2}|{3}\n'.format(project_uuid, linkage, config, arch))
        output.write('\tEndGlobalSection\n')

def build_solution_nested_projects(output, nested_projects):
    if len(nested_projects) >= 1:
        output.write('\tGlobalSection(NestedProjects) = preSolution\n')
        for project_uuid, parent_project_uuid in nested_projects:
            output.write('\t\t{{{0}}} = {{{1}}}\n'.format(project_uuid, parent_project_uuid))
        output.write('\tEndGlobalSection\n')

###---==============================================================---
###           CLEAN SOLUTION/PROJECT FILES & DIRECTORIES
###---==============================================================---

def clean_all():
    clean_unlink('ipch')
    clean_unlink('Upcaste.opensdf')
    clean_unlink('Upcaste.sln')
    clean_unlink('Upcaste.sdf')
    clean_unlink('Upcaste.suo')
    clean_unlink('Upcaste.v11.suo')
    for project_name, project_info in projects.items():
        project_file = get_project_filepath(project_name, project_info)
        clean_unlink(project_file)
        clean_unlink(project_file + '.filters')
        clean_unlink(project_file + '.user')

def clean_unlink(path):
    if os.path.isdir(path):
        clean_directory(path)
        os.rmdir(path)
    elif not os.path.islink(path) and os.path.isfile(path):
        clean_remove_permissions(path)
        os.remove(path)

def clean_directory(path):
    if not os.path.isdir(path):
        return
    clean_remove_permissions(path)
    for name in os.listdir(path):
        filepath = os.path.join(path, name)
        if not os.path.islink(filepath) and os.path.isdir(filepath):
            clean_unlink(filepath)
        else:
            clean_remove_permissions(filepath)
            os.remove(filepath)

def clean_remove_permissions(path):
    if (not os.access(path, os.W_OK)):
        os.chmod(path, stat.S_IWRITE | stat.S_IREAD)

###---==============================================================---
###                               MAIN
###---==============================================================---

def main():
    clean_all()
    project_uuids = build_solution_file()
    build_project_files(project_uuids)

if __name__ == '__main__':
    main()
