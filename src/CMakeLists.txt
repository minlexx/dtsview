set(dtsview_SRCS
    main.cpp
    dtsviewwindow.cpp
    dtsviewwindow.h
)

set(libdtc_SOURCES
#    "libdtc/checks.c"
    "libdtc/data.c"
    "libdtc/dtc-lexer.lex.c"
    "libdtc/dtc-parser.tab.c"
    "libdtc/dtc.c"
    "libdtc/flattree.c"
    "libdtc/fstree.c"
    "libdtc/livetree.c"
    "libdtc/srcpos.c"
    "libdtc/treesource.c"
    "libdtc/util.c"
)

set(libdtc_HEADERS
    "libdtc/dtc-parser.tab.h"
    "libdtc/dtc.h"
    "libdtc/fdt.h"
    "libdtc/libfdt_env.h"
    "libdtc/srcpos.h"
    "libdtc/util.h"
    "libdtc/version_gen.h"
)

ki18n_wrap_ui(dtsview_SRCS dtsview.ui)

add_executable(dtsview
    ${dtsview_SRCS}
    ${libdtc_HEADERS}
    ${libdtc_SOURCES}
)

target_link_libraries(dtsview
    KF5::CoreAddons
    KF5::I18n
    Qt5::Widgets
)

install(TARGETS dtsview ${KDE_INSTALL_TARGETS_DEFAULT_ARGS})
install(PROGRAMS org.example.dtsview.desktop DESTINATION ${KDE_INSTALL_APPDIR})
install(FILES org.example.dtsview.appdata.xml DESTINATION ${KDE_INSTALL_METAINFODIR})
