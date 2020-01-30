package(default_visibility = ["//visibility:public"])

licenses(["notice"])

exports_files(["LICENSE"])

# a library of math an statistics utilities, mostly derived from Numerical Recipes in C, but
# changed for SL purposes.

cc_library(
    name = "utility",
    srcs = [
        "src/balanc.c",
        "src/brent.c",
        "src/choldc.c",
        "src/control.c",
        "src/data_sets.c",
        "src/dbrent.c",
        "src/df1dim.c",
        "src/dfpmin.c",
        "src/distributions.c",
        "src/dlinmin.c",
        "src/elmhes.c",
        "src/f1dim.c",
        "src/frprmn.c",
        "src/hqr.c",
        "src/j-fixed.c",
        "src/kdtree.c",
        "src/ldltinv.c",
        "src/linmin.c",
        "src/lookup_table.c",
        "src/lubksb.c",
        "src/ludcmp.c",
        "src/mnbrak.c",
        "src/numerics_package.c",
        "src/parm_opt.c",
        "src/powell.c",
        "src/pythag.c",
        "src/stability.c",
        "src/statistics.c",
        "src/svbksb.c",
        "src/svdcmp.c",
        "src/utility.c",
    ],
    includes = ["include"],
    textual_hdrs = glob(["include/*.h"]),
)

cc_library(
    name = "src/sort",
    srcs = ["src/sort.c"],
)

cc_library(
    name = "src/random",
    srcs = ["src/random.c"],
)

cc_library(
    name = "src/eigenpackage",
    srcs = ["src/eigenpackage.c"],
)
