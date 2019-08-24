#include <stdio.h>

#include "json_demo.h"

#define CMD_CFG_FILE "cmd_json"

#define ITEM_KEY_CMD_ARG    "cmd_arg"
#define ITEM_KEY_CMD_EXEC   "cmd_exec"
#define ITEM_KEY_CMD_TYPE   "cmd_type"



static int cmdReadCfg(const char *file, char *buf, size_t bufsize)
{
    if (!file || !buf || !bufsize)
    {
        return -1;
    }
    
    FILE *fp = NULL;
    if (NULL == (fp = fopen(file, "r")))
    {
        return -1;
    }
    
    char *ptr_read = buf;
    size_t read_len = 0;
    size_t read_cnt = 0;

    while (read_len < bufsize && (read_cnt = fread(ptr_read, 1, bufsize - read_len - 1, fp)) > 0)
    { 
        ptr_read += read_cnt * 1;
        read_len += read_cnt * 1;
    }
    
    buf[read_len] = '\0';
    
    fclose(fp);
    return 0;
}

int main(int argc, char *argv[])
{
    char cmd_file[8192] = { 0 };
    
    
    /* 读取配置文件 */
    cmdReadCfg(CMD_CFG_FILE, cmd_file, sizeof(cmd_file));
    
    /* 根据json字符串创建json对象 */
    struct json_object *objCmd = json_tokener_parse(cmd_file);
    if (!objCmd || is_error(objCmd))
    {
        return -1;
    }
    
    /* 检查是否是数组 */
    if (!IS_JSON_TYPE(json_type_array, objCmd))
    {
        return -1;
    }
    
    /* 命令个数 */
    int cmd_cnt = json_object_array_length(objCmd);
    
    int idx_cmd;
    for (idx_cmd = 0; idx_cmd < cmd_cnt; ++idx_cmd)
    {
        struct json_object *cmd_item = json_object_array_get_idx(objCmd, idx_cmd);
        if (!IS_JSON_TYPE(json_type_object, cmd_item))
        {
            continue;
        }
        
        //cmd_arg
        struct json_object *obj_cmd_arg = json_object_object_get(cmd_item, ITEM_KEY_CMD_ARG);
        if (!IS_JSON_TYPE(json_type_array, obj_cmd_arg))
        {
            continue;
        }
        int cmd_arg_cnt = json_object_array_length(obj_cmd_arg);
        int idx_arg;
        for (idx_arg = 0; idx_arg < cmd_arg_cnt; ++idx_arg)
        {
            printf("cmd_arg-%d : %s\n", idx_arg, json_object_get_string(json_object_array_get_idx(obj_cmd_arg, idx_arg)));
        }
        
        //cmd_exec
        struct json_object *obj_cmd_exec = json_object_object_get(cmd_item, ITEM_KEY_CMD_EXEC);
        if (!IS_JSON_TYPE(json_type_array, obj_cmd_exec))
        {
            continue;
        }
        int cmd_exec_cnt = json_object_array_length(obj_cmd_exec);
        int idx_exec;
        for (idx_exec = 0; idx_exec < cmd_exec_cnt; ++idx_exec)
        {
            printf("cmd_exec-%d : %s\n", idx_exec, json_object_get_string(json_object_array_get_idx(obj_cmd_exec, idx_exec)));
        }
        
        //cmd_type
        struct json_object *obj_cmd_type = json_object_object_get(cmd_item, ITEM_KEY_CMD_TYPE);
        if (!IS_JSON_TYPE(json_type_string, obj_cmd_type))
        {
            continue;
        }
        printf("cmd_type : %s\n", json_object_get_string(obj_cmd_type));
    }
    return 0;
}
