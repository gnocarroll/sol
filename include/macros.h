#pragma once

#define DEF_DERIVED_TYPES(name) \
    using Optional##name = std::optional< name >; \
    using Optional##name##Ref = std::optional<std::reference_wrapper< name > >; \
    \
    using name##Ptr = std::unique_ptr< name >; \
    using Optional##name##Ptr = std::optional< name##Ptr >; \
    template <typename E> \
    using Expected##name##Ptr = std::expected< name##Ptr ,E>; \
    \
    using name##PtrVec = std::vector<std::unique_ptr< name > >;