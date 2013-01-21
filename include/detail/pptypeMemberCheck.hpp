/*
 * pptypeMemberCheck.hpp
 *
 * Helper file for generating all of the argument lists for argument checks.
 */
#ifndef PREPROCESSOR_TYPES_MEMBER_CHECK_H_
#define PREPROCESSOR_TYPES_MEMBER_CHECK_H_

#define MEMBER_NO_CONSTRUCT_ARG(arg) *((arg*)NULL)

#define MEMBER_CHECK_1_TYPENAME typename A1
#define MEMBER_CHECK_1_CARGLIST R(A1)
#define MEMBER_CHECK_1_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1)
#define MEMBER_CHECK_1_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_1_TYPENAME,                                \
                MEMBER_CHECK_1_CARGLIST, MEMBER_CHECK_1_FARGLIST)


#define MEMBER_CHECK_2_TYPENAME typename A1, typename A2
#define MEMBER_CHECK_2_CARGLIST R(A1, A2)
#define MEMBER_CHECK_2_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2)
#define MEMBER_CHECK_2_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_2_TYPENAME,                                \
                MEMBER_CHECK_2_CARGLIST, MEMBER_CHECK_2_FARGLIST)


#define MEMBER_CHECK_3_TYPENAME typename A1, typename A2, typename A3
#define MEMBER_CHECK_3_CARGLIST R(A1, A2, A3)
#define MEMBER_CHECK_3_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3)
#define MEMBER_CHECK_3_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_3_TYPENAME,                                \
                MEMBER_CHECK_3_CARGLIST, MEMBER_CHECK_3_FARGLIST)


#define MEMBER_CHECK_4_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4
#define MEMBER_CHECK_4_CARGLIST R(A1, A2, A3, A4)
#define MEMBER_CHECK_4_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4)
#define MEMBER_CHECK_4_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_4_TYPENAME,                                \
                MEMBER_CHECK_4_CARGLIST, MEMBER_CHECK_4_FARGLIST)


#define MEMBER_CHECK_5_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5
#define MEMBER_CHECK_5_CARGLIST R(A1, A2, A3, A4, A5)
#define MEMBER_CHECK_5_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5)
#define MEMBER_CHECK_5_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_5_TYPENAME,                                \
                MEMBER_CHECK_5_CARGLIST, MEMBER_CHECK_5_FARGLIST)


#define MEMBER_CHECK_5_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5
#define MEMBER_CHECK_5_CARGLIST R(A1, A2, A3, A4, A5)
#define MEMBER_CHECK_5_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5)
#define MEMBER_CHECK_5_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_5_TYPENAME,                                \
                MEMBER_CHECK_5_CARGLIST, MEMBER_CHECK_5_FARGLIST)


#define MEMBER_CHECK_6_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5, typename A6
#define MEMBER_CHECK_6_CARGLIST R(A1, A2, A3, A4, A5, A6)
#define MEMBER_CHECK_6_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A6)
#define MEMBER_CHECK_6_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_6_TYPENAME,                                \
                MEMBER_CHECK_6_CARGLIST, MEMBER_CHECK_6_FARGLIST)


#define MEMBER_CHECK_7_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5, typename A6,                      \
                                typename A7
#define MEMBER_CHECK_7_CARGLIST R(A1, A2, A3, A4, A5, A6, A7)
#define MEMBER_CHECK_7_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A6),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A7)
#define MEMBER_CHECK_7_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_7_TYPENAME,                                \
                MEMBER_CHECK_7_CARGLIST, MEMBER_CHECK_7_FARGLIST)


#define MEMBER_CHECK_8_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5, typename A6,                      \
                                typename A7, typename A8
#define MEMBER_CHECK_8_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8)
#define MEMBER_CHECK_8_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A6),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A7),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A8)
#define MEMBER_CHECK_8_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_8_TYPENAME,                                \
                MEMBER_CHECK_8_CARGLIST, MEMBER_CHECK_8_FARGLIST)


#define MEMBER_CHECK_9_TYPENAME typename A1, typename A2, typename A3,                      \
                                typename A4, typename A5, typename A6,                      \
                                typename A7, typename A8, typename A9
#define MEMBER_CHECK_9_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9)
#define MEMBER_CHECK_9_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A2),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A3),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A4),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A5),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A6),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A7),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A8),                                \
                                MEMBER_NO_CONSTRUCT_ARG(A9)
#define MEMBER_CHECK_9_ARG(mname)                                                           \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_9_TYPENAME,                                \
                MEMBER_CHECK_9_CARGLIST, MEMBER_CHECK_9_FARGLIST)


#define MEMBER_CHECK_10_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10
#define MEMBER_CHECK_10_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10)
#define MEMBER_CHECK_10_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10)
#define MEMBER_CHECK_10_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_10_TYPENAME,                               \
                MEMBER_CHECK_10_CARGLIST, MEMBER_CHECK_10_FARGLIST)


#define MEMBER_CHECK_11_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10, typename A11
#define MEMBER_CHECK_11_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10, A11)
#define MEMBER_CHECK_11_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A11)
#define MEMBER_CHECK_11_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_11_TYPENAME,                               \
                MEMBER_CHECK_11_CARGLIST, MEMBER_CHECK_11_FARGLIST)


#define MEMBER_CHECK_12_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10, typename A11, typename A12
#define MEMBER_CHECK_12_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10, A11, A12)
#define MEMBER_CHECK_12_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A11),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A12)
#define MEMBER_CHECK_12_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_12_TYPENAME,                               \
                MEMBER_CHECK_12_CARGLIST, MEMBER_CHECK_12_FARGLIST)


#define MEMBER_CHECK_13_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10, typename A11, typename A12,                  \
                                 typename A13
#define MEMBER_CHECK_13_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10, A11, A12, A13)
#define MEMBER_CHECK_13_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A11),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A12),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A13)
#define MEMBER_CHECK_13_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_13_TYPENAME,                               \
                MEMBER_CHECK_13_CARGLIST, MEMBER_CHECK_13_FARGLIST)


#define MEMBER_CHECK_14_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10, typename A11, typename A12,                  \
                                 typename A13, typename A14
#define MEMBER_CHECK_14_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10, A11, A12, A13, A14)
#define MEMBER_CHECK_14_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A11),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A12),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A13),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A14)
#define MEMBER_CHECK_14_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_14_TYPENAME,                               \
                MEMBER_CHECK_14_CARGLIST, MEMBER_CHECK_14_FARGLIST)


#define MEMBER_CHECK_15_TYPENAME typename A1, typename A2, typename A3,                     \
                                 typename A4, typename A5, typename A6,                     \
                                 typename A7, typename A8, typename A9,                     \
                                 typename A10, typename A11, typename A12,                  \
                                 typename A13, typename A14, typename A15
#define MEMBER_CHECK_15_CARGLIST R(A1, A2, A3, A4, A5, A6, A7, A8, A9,                      \
                                   A10, A11, A12, A13, A14, A15)
#define MEMBER_CHECK_15_FARGLIST MEMBER_NO_CONSTRUCT_ARG(A1),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A2),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A3),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A4),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A5),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A6),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A7),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A8),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A9),                               \
                                 MEMBER_NO_CONSTRUCT_ARG(A10),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A11),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A12),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A13),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A14),                              \
                                 MEMBER_NO_CONSTRUCT_ARG(A15)
#define MEMBER_CHECK_15_ARG(mname)                                                          \
        MEMBER_CHECK_ARG_GEN(mname, MEMBER_CHECK_15_TYPENAME,                               \
                MEMBER_CHECK_15_CARGLIST, MEMBER_CHECK_15_FARGLIST)

#endif /* PREPROCESSOR_TYPES_MEMBER_CHECK_H_ */
