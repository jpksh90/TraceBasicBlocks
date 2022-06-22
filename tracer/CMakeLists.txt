add_library(Tracer MODULE tracer.cpp)
if(APPLE)
    set_target_properties(Tracer PROPERTIES LINK_FLAGS "-undefined dynamic_lookup")
endif(APPLE)