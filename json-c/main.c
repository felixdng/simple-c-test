#include <stdio.h>

#include "json_demo.h"

const char str_1[] = "[{ \"type\":\"1\", \"tude_msg\": { \"lotude_int\":\"[7-9] [5-9]\", \"lotude_dec\":\"[0-9] [0-9] [0-9] [0-9] [0-9] [0-9]\",\"lotude_spr\":\"|\",\"latude_spr\":\"|\",\"latude_int\":\"[0-4] [0-9]\",\"latude_dec\":\"[0-9] [0-9] [0-9] [0-9] [0-9] [0-9]\"}},{\"type\":\"0\",\"mobile_msg\":\"1[38][0-9]12345678\"}]";

int main(int argc, char *argv[])
{
    demo_parse_json(str_1);

    return 0;
}
