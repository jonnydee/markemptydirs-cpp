OTHER_FILES += \
    win32/build_product.bat

product.commands += pushd $$shell_path($${_PRO_FILE_PWD_}/win32) & build_product.bat $$VERSION_FULL & popd

QMAKE_EXTRA_TARGETS += product
