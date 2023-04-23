#pragma once

#define UNPACK(...) __VA_ARGS__
#define YL_COMPONENT(componentStruct, componentName) \
    UNPACK componentStruct; \

#define YL_REFLECT(typeName, data_member) \
    entt::meta<typeName>() \
        .type(entt::hashed_string(typeid(typeName).name())) \
        .data<&typeName::data_member>(entt::hashed_string(#data_member))
