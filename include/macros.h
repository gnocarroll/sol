#pragma once

#define DEF_PTR_TYPES(name) \
    using name##Ptr = std::unique_ptr< name >; \
    using Optional##name##Ptr = std::optional< name##Ptr >; \
    template <typename E> \
    using Expected##name##Ptr = std::expected< name##Ptr ,E>;