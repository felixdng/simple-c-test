#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#include "json_demo.h"


int demo_parse_json(const char *str)
{
    /* 根据json字符串创建json对象 */
    struct json_object *jsonObjModel = json_tokener_parse((const char *)str);
    if (!jsonObjModel || is_error(jsonObjModel))
    {
        printf("ERROR[%s:%u] invalid json string!\n", __func__, __LINE__);
        return -1;
    }

    /* 先解析type */
    struct json_object *jsonObjModelType = json_object_object_get(jsonObjModel, "type");
    if (!jsonObjModelType)
    {
        printf("ERROR[%s:%u] has no \"type\"!\n", __func__, __LINE__);
        json_object_put(jsonObjModel);
        return -1;
    }
    const char *type = json_object_get_string(jsonObjModelType);
    printf("type=%s\n", type);

    json_object_put(jsonObjModel);
    return 0;
}


