#ifndef __JSON_DEMO_H__
#define __JSON_DEMO_H__

#include "json/json.h"



#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif /* __cplusplus */

inline int IS_JSON_TYPE(enum json_type obj_type, struct json_object *obj)
{
    return (obj_type == json_object_get_type(obj)) ? 1 : 0;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* __cplusplus */

#endif /* __JSON_DEMO_H__ */

