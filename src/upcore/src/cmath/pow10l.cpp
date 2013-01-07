//
//  Upcaste Performance Libraries
//  Copyright (C) 2012-2013 Jesse W. Towner
//
//  Permission is hereby granted, free of charge, to any person obtaining
//  a copy of this software and associated documentation files (the
//  "Software"), to deal in the Software without restriction, including
//  without limitation the rights to use, copy, modify, merge, publish,
//  distribute, sublicense, and/or sell copies of the Software, and to
//  permit persons to whom the Software is furnished to do so, subject to
//  the following conditions:
//
//  The above copyright notice and this permission notice shall be
//  included in all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include <up/cmath.hpp>

namespace up { namespace math
{
    namespace
    {
        alignas(UP_MAX_CACHE_LINE_SIZE) long double const pow10l_table[] =
        {
	          1e0l,   1e1l,   1e2l,   1e3l,   1e4l,   1e5l,   1e6l,   1e7l,   1e8l,   1e9l,
	         1e10l,  1e11l,  1e12l,  1e13l,  1e14l,  1e15l,  1e16l,  1e17l,  1e18l,  1e19l,  
	         1e20l,  1e21l,  1e22l,  1e23l,  1e24l,  1e25l,  1e26l,  1e27l,  1e28l,  1e29l,  
	         1e30l,  1e31l,  1e32l,  1e33l,  1e34l,  1e35l,  1e36l,  1e37l,  1e38l,  1e39l,  
	         1e40l,  1e41l,  1e42l,  1e43l,  1e44l,  1e45l,  1e46l,  1e47l,  1e48l,  1e49l,  
	         1e50l,  1e51l,  1e52l,  1e53l,  1e54l,  1e55l,  1e56l,  1e57l,  1e58l,  1e59l,  
	         1e60l,  1e61l,  1e62l,  1e63l,  1e64l,  1e65l,  1e66l,  1e67l,  1e68l,  1e69l,  
	         1e70l,  1e71l,  1e72l,  1e73l,  1e74l,  1e75l,  1e76l,  1e77l,  1e78l,  1e79l,  
	         1e80l,  1e81l,  1e82l,  1e83l,  1e84l,  1e85l,  1e86l,  1e87l,  1e88l,  1e89l,  
	         1e90l,  1e91l,  1e92l,  1e93l,  1e94l,  1e95l,  1e96l,  1e97l,  1e98l,  1e99l,
	        1e100l, 1e101l, 1e102l, 1e103l, 1e104l, 1e105l, 1e106l, 1e107l, 1e108l, 1e109l, 
	        1e110l, 1e111l, 1e112l, 1e113l, 1e114l, 1e115l, 1e116l, 1e117l, 1e118l, 1e119l, 
	        1e120l, 1e121l, 1e122l, 1e123l, 1e124l, 1e125l, 1e126l, 1e127l, 1e128l, 1e129l, 
	        1e130l, 1e131l, 1e132l, 1e133l, 1e134l, 1e135l, 1e136l, 1e137l, 1e138l, 1e139l, 
	        1e140l, 1e141l, 1e142l, 1e143l, 1e144l, 1e145l, 1e146l, 1e147l, 1e148l, 1e149l, 
	        1e150l, 1e151l, 1e152l, 1e153l, 1e154l, 1e155l, 1e156l, 1e157l, 1e158l, 1e159l,
            1e160l, 1e161l, 1e162l, 1e163l, 1e164l, 1e165l, 1e166l, 1e167l, 1e168l, 1e169l,
            1e170l, 1e171l, 1e172l, 1e173l, 1e174l, 1e175l, 1e176l, 1e177l, 1e178l, 1e179l,
            1e180l, 1e181l, 1e182l, 1e183l, 1e184l, 1e185l, 1e186l, 1e187l, 1e188l, 1e189l,
            1e190l, 1e191l, 1e192l, 1e193l, 1e194l, 1e195l, 1e196l, 1e197l, 1e198l, 1e199l,
	        1e200l, 1e201l, 1e202l, 1e203l, 1e204l, 1e205l, 1e206l, 1e207l, 1e208l, 1e209l, 
	        1e210l, 1e211l, 1e212l, 1e213l, 1e214l, 1e215l, 1e216l, 1e217l, 1e218l, 1e219l, 
	        1e220l, 1e221l, 1e222l, 1e223l, 1e224l, 1e225l, 1e226l, 1e227l, 1e228l, 1e229l, 
#if LDBL_MAX_10_EXP >= 500
            1e230l, 1e231l, 1e232l, 1e233l, 1e234l, 1e235l, 1e236l, 1e237l, 1e238l, 1e239l, 
	        1e240l, 1e241l, 1e242l, 1e243l, 1e244l, 1e245l, 1e246l, 1e247l, 1e248l, 1e249l, 
	        1e250l, 1e251l, 1e252l, 1e253l, 1e254l, 1e255l, 1e256l, 1e257l, 1e258l, 1e259l,
	        1e260l, 1e261l, 1e262l, 1e263l, 1e264l, 1e265l, 1e266l, 1e267l, 1e268l, 1e269l,
            1e270l, 1e271l, 1e272l, 1e273l, 1e274l, 1e275l, 1e276l, 1e277l, 1e278l, 1e279l,
            1e280l, 1e281l, 1e282l, 1e283l, 1e284l, 1e285l, 1e286l, 1e287l, 1e288l, 1e289l,
            1e290l, 1e291l, 1e292l, 1e293l, 1e294l, 1e295l, 1e296l, 1e297l, 1e298l, 1e299l,
	        1e300l, 1e301l, 1e302l, 1e303l, 1e304l, 1e305l, 1e306l, 1e307l, 1e308l, 1e309l, 
            1e310l, 1e311l, 1e312l, 1e313l, 1e314l, 1e315l, 1e316l, 1e317l, 1e318l, 1e319l, 
	        1e320l, 1e321l, 1e322l, 1e323l, 1e324l, 1e325l, 1e326l, 1e327l, 1e328l, 1e329l, 
	        1e330l, 1e331l, 1e332l, 1e333l, 1e334l, 1e335l, 1e336l, 1e337l, 1e338l, 1e339l, 
	        1e340l, 1e341l, 1e342l, 1e343l, 1e344l, 1e345l, 1e346l, 1e347l, 1e348l, 1e349l, 
	        1e350l, 1e351l, 1e352l, 1e353l, 1e354l, 1e355l, 1e356l, 1e357l, 1e358l, 1e359l,
	        1e360l, 1e361l, 1e362l, 1e363l, 1e364l, 1e365l, 1e366l, 1e367l, 1e368l, 1e369l,
            1e370l, 1e371l, 1e372l, 1e373l, 1e374l, 1e375l, 1e376l, 1e377l, 1e378l, 1e379l,
            1e380l, 1e381l, 1e382l, 1e383l, 1e384l, 1e385l, 1e386l, 1e387l, 1e388l, 1e389l,
            1e390l, 1e391l, 1e392l, 1e393l, 1e394l, 1e395l, 1e396l, 1e397l, 1e398l, 1e399l,
	        1e400l, 1e401l, 1e402l, 1e403l, 1e404l, 1e405l, 1e406l, 1e407l, 1e408l, 1e409l, 
	        1e410l, 1e411l, 1e412l, 1e413l, 1e414l, 1e415l, 1e416l, 1e417l, 1e418l, 1e419l, 
	        1e420l, 1e421l, 1e422l, 1e423l, 1e424l, 1e425l, 1e426l, 1e427l, 1e428l, 1e429l, 
	        1e430l, 1e431l, 1e432l, 1e433l, 1e434l, 1e435l, 1e436l, 1e437l, 1e438l, 1e439l, 
	        1e440l, 1e441l, 1e442l, 1e443l, 1e444l, 1e445l, 1e446l, 1e447l, 1e448l, 1e449l, 
	        1e450l, 1e451l, 1e452l, 1e453l, 1e454l, 1e455l, 1e456l, 1e457l, 1e458l, 1e459l,
	        1e460l, 1e461l, 1e462l, 1e463l, 1e464l, 1e465l, 1e466l, 1e467l, 1e468l, 1e469l,
            1e470l, 1e471l, 1e472l, 1e473l, 1e474l, 1e475l, 1e476l, 1e477l, 1e478l, 1e479l,
            1e480l, 1e481l, 1e482l, 1e483l, 1e484l, 1e485l, 1e486l, 1e487l, 1e488l, 1e489l,
            1e490l, 1e491l, 1e492l, 1e493l, 1e494l, 1e495l, 1e496l, 1e497l, 1e498l, 1e499l
#endif
        };

        constexpr int pow10l_table_size = static_cast<int>(sizeof(pow10l_table) / sizeof(pow10l_table[0]));
    }

    LIBUPCOREAPI
    long double pow10l(int n) noexcept {
        bool negative;
        long double r;

        negative = false;
        if (n < 0) {
            negative = true;
            n = -n;
        }

        if (UPLIKELY(n < pow10l_table_size)) {
            r = pow10l_table[n];
        }
        else {
            r = pow10l_table[pow10l_table_size - 1];
            for (;;) {
                n -= pow10l_table_size - 1;
                if (n < pow10l_table_size) {
                    r *= pow10l_table[n];
                    break;
                }
                r *= pow10l_table[pow10l_table_size - 1];
            }
        }

        return negative ? (1.0l / r) : r;
    }
}}
