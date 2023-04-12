#pragma once

#define YL_REFLECT(typename, ...) \
    entt::meta<typename>() \
        .type(entt::hashed_string(#typename)) \
        INTERNAL_REFLECT_DATA(typename, __VA_ARGS__)

#define INTERNAL_REFLECT_DATA(typename, ...) \
    INTERNAL_REFLECT_DATA_IMPL(typename, __VA_ARGS__)

#define INTERNAL_REFLECT_DATA_IMPL(typename, data_member, ...) \
    .data<&typename::data_member>(entt::hashed_string(#data_member)) \
    INTERNAL_REFLECT_DATA_IMPL(typename, __VA_ARGS__)

#define INTERNAL_REFLECT_DATA_IMPL(typename, data_member) \
    .data<&typename::data_member>(entt::hashed_string(#data_member))