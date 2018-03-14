include(bde_package)

bde_prefixed_override(bslstl process_package)
function(bslstl_process_package retPackage)
    process_package_base("" package ${ARGN})

    bde_struct_get_field(testInterface ${package} TEST_INTERFACE_TARGET)
    bde_interface_target_compile_options(
        ${testInterface}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:
                /bigobj
            >
    )

    bde_struct_get_field(objlib ${package} OBJ_TARGET)
    bde_struct_get_field(tests ${package} TEST_TARGETS)
    set_target_properties(${objlib} ${tests} PROPERTIES SUPPRESS_BSL_OVERRIDES_STD ON)

    bde_return(${package})
endfunction()