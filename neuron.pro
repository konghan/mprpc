TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS=-L/usr/local/lib -lpthread -levent
INCLUDEPATH += "/usr/local/include"

SOURCES += main.c \
    fixarray.c \
    neu-pdu.c \
    neu-service.c \
    neu-session.c \
    neu-worker.c \
    neulog.c \
    idl-compiler/generate/t_as3_generator.cc \
    idl-compiler/generate/t_c_glib_generator.cc \
    idl-compiler/generate/t_cocoa_generator.cc \
    idl-compiler/generate/t_cpp_generator.cc \
    idl-compiler/generate/t_csharp_generator.cc \
    idl-compiler/generate/t_d_generator.cc \
    idl-compiler/generate/t_delphi_generator.cc \
    idl-compiler/generate/t_erl_generator.cc \
    idl-compiler/generate/t_generator.cc \
    idl-compiler/generate/t_go_generator.cc \
    idl-compiler/generate/t_gv_generator.cc \
    idl-compiler/generate/t_hs_generator.cc \
    idl-compiler/generate/t_html_generator.cc \
    idl-compiler/generate/t_java_generator.cc \
    idl-compiler/generate/t_javame_generator.cc \
    idl-compiler/generate/t_js_generator.cc \
    idl-compiler/generate/t_json_generator.cc \
    idl-compiler/generate/t_lua_generator.cc \
    idl-compiler/generate/t_ocaml_generator.cc \
    idl-compiler/generate/t_perl_generator.cc \
    idl-compiler/generate/t_php_generator.cc \
    idl-compiler/generate/t_py_generator.cc \
    idl-compiler/generate/t_rb_generator.cc \
    idl-compiler/generate/t_st_generator.cc \
    idl-compiler/generate/t_xsd_generator.cc \
    idl-compiler/parse/parse.cc \
    idl-compiler/parse/t_typedef.cc \
    idl-compiler/main.cc \
    client/neucli.c \
    common/fixarray-test.c \
    common/fixarray.c \
    common/neu-pdu.c \
    common/neulog.c \
    gen-srv/redis-rpc-gen.c \
    gen-srv/redis-rpc-gen.skeleton.c \
    gen-srv/redis-struct-gen.c \
    gen-srv/redis.skeleton.c \
    idl-compiler/md5.c \
    server/main.c \
    server/neu-service.c \
    server/neu-session.c \
    server/neu-worker.c

include(deployment.pri)
qtcAddDeployment()

DISTFILES += \
    logger.js \
    neuron.pro.user \
    LICENSE \
    Makefile \
    README.md \
    spec.md

HEADERS += \
    fixarray.h \
    neu-handle.h \
    neu-msg.h \
    neu-pdu.h \
    neu-service.h \
    neu-session.h \
    neu-worker.h \
    neulog.h \
    neuron.h \
    client/neucli.h \
    common/fixarray.h \
    common/neu-handle.h \
    common/neu-msg.h \
    common/neu-pdu.h \
    common/neulog.h \
    common/neuron.h \
    gen-srv/neurpc.h \
    gen-srv/redis-rpc-gen.h \
    gen-srv/redis-struct-gen.h \
    idl-compiler/generate/t_generator.h \
    idl-compiler/generate/t_generator_registry.h \
    idl-compiler/generate/t_html_generator.h \
    idl-compiler/generate/t_oop_generator.h \
    idl-compiler/parse/t_array.h \
    idl-compiler/parse/t_base_type.h \
    idl-compiler/parse/t_const.h \
    idl-compiler/parse/t_const_value.h \
    idl-compiler/parse/t_container.h \
    idl-compiler/parse/t_doc.h \
    idl-compiler/parse/t_enum.h \
    idl-compiler/parse/t_enum_value.h \
    idl-compiler/parse/t_field.h \
    idl-compiler/parse/t_function.h \
    idl-compiler/parse/t_list.h \
    idl-compiler/parse/t_map.h \
    idl-compiler/parse/t_program.h \
    idl-compiler/parse/t_scope.h \
    idl-compiler/parse/t_service.h \
    idl-compiler/parse/t_set.h \
    idl-compiler/parse/t_struct.h \
    idl-compiler/parse/t_topic.h \
    idl-compiler/parse/t_type.h \
    idl-compiler/parse/t_typedef.h \
    idl-compiler/windows/config.h \
    idl-compiler/windows/version.h \
    idl-compiler/globals.h \
    idl-compiler/logging.h \
    idl-compiler/main.h \
    idl-compiler/md5.h \
    idl-compiler/platform.h \
    idl-compiler/version.h \
    server/neu-service.h \
    server/neu-session.h \
    server/neu-worker.h

