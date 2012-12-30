#!/usr/bin/env python3.1

#
# Upcaste Performance Foundation
# Copyright (C) 2012 Jesse W. Towner
#
# Permission is hereby granted, free of charge, to any person obtaining
# a copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
#
# The above copyright notice and this permission notice shall be
# included in all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#

# This python script uses the Jinja2 templating framework to generate the
# following source and header files in libupcore:
#
# - include/upcore/up/math/mathfwd.hpp
# - include/upcore/up/math/vectors.hpp
# - include/upcore/up/math/matrices.hpp
# - include/upcore/up/math/int4.hpp
# - include/upcore/up/math/uint4.hpp
# - include/upcore/up/math/float4.hpp
# - include/upcore/up/math/generic/vectors.inl
# - include/upcore/up/math/generic/scalar_operations.inl
# - include/upcore/up/math/generic/vector_conversion_2.inl
# - include/upcore/up/math/generic/vector_conversion_3.inl
# - include/upcore/up/math/generic/vector_conversion_4.inl
# - include/upcore/up/math/generic/int2_type.inl
# - include/upcore/up/math/generic/int3_type.inl
# - include/upcore/up/math/generic/int4_type.inl
# - include/upcore/up/math/generic/uint2_type.inl
# - include/upcore/up/math/generic/uint3_type.inl
# - include/upcore/up/math/generic/uint4_type.inl
# - include/upcore/up/math/generic/float2_type.inl
# - include/upcore/up/math/generic/float3_type.inl
# - include/upcore/up/math/generic/float4_type.inl
# - include/upcore/up/math/generic/long2_type.inl
# - include/upcore/up/math/generic/long3_type.inl
# - include/upcore/up/math/generic/long4_type.inl
# - include/upcore/up/math/generic/ulong2_type.inl
# - include/upcore/up/math/generic/ulong3_type.inl
# - include/upcore/up/math/generic/ulong4_type.inl
# - include/upcore/up/math/generic/double2_type.inl
# - include/upcore/up/math/generic/double3_type.inl
# - include/upcore/up/math/generic/double4_type.inl
# - include/upcore/up/math/generic/int4_operations.inl
# - include/upcore/up/math/generic/uint4_operations.inl
# - include/upcore/up/math/generic/float4_operations.inl
# - src/upcore/src/math/vectors.cpp
# - src/upcore/src/math/float4.cpp
# - src/upcore/src/math/double4.cpp

import codecs
import jinja2
import os.path

# various directories
root_dir = os.path.abspath(os.path.join(os.pardir, os.pardir, os.pardir))
include_dir = os.path.join(root_dir, "include", "upcore")
source_dir = os.path.join(root_dir, "src", "upcore")
math_include_dir = os.path.join(include_dir, "up", "math")
math_source_dir = os.path.join(source_dir, "src", "math")
jinja_env = jinja2.Environment(loader=jinja2.FileSystemLoader(os.path.join(source_dir, "gen")))

def render_template(templatename, outputpath, **params):
    print("generating '{0}'".format(os.path.abspath(outputpath)))
    template = jinja_env.get_template(templatename)
    with codecs.open(outputpath, "w", encoding="utf_8") as outputfile:
        outputfile.write(template.render(**params))
        outputfile.write("\n")

render_template("mathfwd.hpp.template", os.path.join(math_include_dir, "mathfwd.hpp"))
render_template("generic_scalar_operations.inl.template", os.path.join(math_include_dir, "generic", "scalar_operations.inl"))

# set of different sized vectors we support
vector32_element_types = ['int', 'uint', 'float']
vector64_element_types = ['long', 'ulong', 'double']
all_element_types = vector32_element_types + vector64_element_types
all_vector_dims = [2, 3, 4]
all_matrix_rows = [2, 3, 4]
op_vector_dims = [4]

vector32_types = []
for element_type in vector32_element_types:
    for dim in all_vector_dims:
        vector32_types.append((element_type, dim))

vector64_types = []
for element_type in vector64_element_types:
    for dim in all_vector_dims:
        vector64_types.append((element_type, dim))

op_vector32_types = []
op_matrix32_types = []
for element_type in vector32_element_types:
    for dim in op_vector_dims:
        op_vector32_types.append((element_type, dim))
        for dim2 in all_matrix_rows:
            op_matrix32_types.append((element_type, dim, dim2))

op_vector64_types = []
op_matrix64_types = []
for element_type in vector64_element_types:
    for dim in op_vector_dims:
        op_vector64_types.append((element_type, dim))
        for dim2 in all_matrix_rows:
            op_matrix64_types.append((element_type, dim, dim2))

all_vector_types = vector32_types + vector64_types
op_vector_types = op_vector32_types + op_vector64_types
op_matrix_types = op_matrix32_types + op_matrix64_types

# generate the generic vector headers and source files
render_template("matrices.hpp.template", os.path.join(math_include_dir, "matrices.hpp"), vector_types=op_vector_types)
render_template("vectors.hpp.template", os.path.join(math_include_dir, "vectors.hpp"), vector_types=op_vector_types, all_vector_types=all_vector_types)
render_template("vectors.cpp.template", os.path.join(math_source_dir, "vectors.cpp"), vector_types=op_vector32_types)
render_template("generic_vectors.inl.template", os.path.join(math_include_dir, "generic", "vectors.inl"), vector_types=all_vector_types, dimensions=all_vector_dims)

for element_type, dim in all_vector_types:
    vector_type = "{0}{1}".format(element_type, dim)
    render_template("generic_vector_type.inl.template", os.path.join(math_include_dir, "generic", vector_type + "_type.inl"), element_type=element_type, dim=dim)
    
for element_type, dim in op_vector32_types:
    vector_type = "{0}{1}".format(element_type, dim)
    render_template("generic_vector_operations.inl.template", os.path.join(math_include_dir, "generic", vector_type + "_operations.inl"), element_type=element_type, dim=dim)
    render_template("vector_type.hpp.template", os.path.join(math_include_dir, vector_type + ".hpp"), element_type=element_type, dim=dim)
    if element_type in ['float']:
        render_template("vector_type.cpp.template", os.path.join(math_source_dir, vector_type + ".cpp"), element_type=element_type, dim=dim)

for element_type, cols, rows in op_matrix32_types:
    matrix_type = "{0}{1}x{2}".format(element_type, cols, rows)
    render_template("matrix_type.hpp.template", os.path.join(math_include_dir, matrix_type + ".hpp"), element_type=element_type, cols=cols, rows=rows)
