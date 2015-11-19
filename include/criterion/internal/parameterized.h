/*
 * The MIT License (MIT)
 *
 * Copyright © 2015 Franklin "Snaipe" Mathieu <http://snai.pe/>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CRITERION_INTERNAL_PARAMETERIZED_H_
# define CRITERION_INTERNAL_PARAMETERIZED_H_

# include "test.h"
# include "../types.h"

# ifdef __cplusplus
#  define CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name) \
    extern "C" void CR_IDENTIFIER_(Category, Name, impl)(Param)
# else
#  define CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name) \
    void CR_IDENTIFIER_(Category, Name, impl)(Param)
# endif

# define CR_PARAM_TEST_BASE(Param, Category, Name, ...)                        \
    CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name);                           \
    struct criterion_test_extra_data CR_IDENTIFIER_(Category, Name, extra) =   \
        CR_EXPAND(CRITERION_MAKE_STRUCT(struct criterion_test_extra_data,      \
            .lang_ = CR_LANG,                                                  \
            .kind_ = CR_TEST_PARAMETERIZED,                                    \
            .param_ = CR_IDENTIFIER_(Category, Name, param),                   \
            .identifier_ = #Category "/" #Name,                                \
            .file_    = __FILE__,                                              \
            .line_    = __LINE__,                                              \
            __VA_ARGS__                                                        \
        ));                                                                    \
    struct criterion_test CR_IDENTIFIER_(Category, Name, meta) = {             \
        #Name,                                                                 \
        #Category,                                                             \
        (void(*)(void)) CR_IDENTIFIER_(Category, Name, impl),                  \
        &CR_IDENTIFIER_(Category, Name, extra)                                 \
    };                                                                         \
    CR_SECTION_("cr_tst")                                                      \
    struct criterion_test *CR_IDENTIFIER_(Category, Name, ptr)                 \
            = &CR_IDENTIFIER_(Category, Name, meta) CR_SECTION_SUFFIX_;        \
    CR_PARAM_TEST_PROTOTYPE_(Param, Category, Name)

# define CR_PARAM_TEST_PARAMS(Category, Name) \
    static struct criterion_test_params CR_IDENTIFIER_(Category, Name, param)(void)

# ifdef __cplusplus
#  define cr_make_param_array(Type, Array, ...) \
    criterion_test_params(sizeof (Type), (Array), __VA_ARGS__)
# else
#  define cr_make_param_array(Type, Array, ...) \
    (struct criterion_test_params) { .size = sizeof (Type), (void*)(Array), __VA_ARGS__ }
# endif

#endif /* !CRITERION_INTERNAL_PARAMETERIZED_H_ */