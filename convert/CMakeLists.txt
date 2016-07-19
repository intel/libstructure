configure_file(convert.hpp ${libstructure_generated_includes_dir}/detail/convert.hpp COPYONLY)

install(FILES convert.hpp
        DESTINATION include/structure/detail
        COMPONENT dev)
