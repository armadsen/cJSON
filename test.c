/*
  Copyright (c) 2009 Dave Gamble

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cJSON.h"

/* Parse text to JSON, then render back to text, and print! */
static void doit(char *text)
{
    char *out = NULL;
    cJSON *json = NULL;

    json = cJSON_Parse(text);
    if (!json)
    {
        printf("Error before: [%s]\n", cJSON_GetErrorPtr());
    }
    else
    {
        out = cJSON_Print(json);
        cJSON_Delete(json);
        printf("%s\n", out);
        free(out);
    }
}

#if 0
/* Read a file, parse, render back, etc. */
static void dofile(char *filename)
{
    FILE *f = NULL;
    long len = 0;
    char *data = NULL;

    /* open in read binary mode */
    f = fopen(filename,"rb");
    /* get the length */
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    fseek(f, 0, SEEK_SET);

    data = (char*)malloc(len + 1);

    fread(data, 1, len, f);
    data[len] = '\0';
    fclose(f);

    doit(data);
    free(data);
}
#endif

/* Used by some code below as an example datatype. */
struct record
{
    const char *precision;
    double lat;
    double lon;
    const char *address;
    const char *city;
    const char *state;
    const char *zip;
    const char *country;
};


/* Create a bunch of objects as demonstration. */
int print_preallocated(cJSON *root)
{
    /* declarations */
    char *out = NULL;
    char *buf = NULL;
    char *buf_fail = NULL;
    int len = 0;
    int len_fail = 0;

    /* formatted print */
    out = cJSON_Print(root);

    /* create buffer to succeed */
    /* the extra 64 bytes are in case a floating point value is printed */
    len = strlen(out) + 64;
    buf = malloc(len);

    /* create buffer to fail */
    len_fail = strlen(out);
    buf_fail = malloc(len_fail);

    /* Print to buffer */
    if (cJSON_PrintPreallocated(root, buf, len, 1) != 0) {
        printf("cJSON_PrintPreallocated failed!\n");
        if (strcmp(out, buf) != 0) {
            printf("cJSON_PrintPreallocated not the same as cJSON_Print!\n");
            printf("cJSON_Print result:\n%s\n", out);
            printf("cJSON_PrintPreallocated result:\n%s\n", buf);
        }
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    /* success */
    printf("%s\n", buf);

    /* force it to fail */
    if (cJSON_PrintPreallocated(root, buf_fail, len_fail, 1) == 0) {
        printf("cJSON_PrintPreallocated failed to show error with insufficient memory!\n");
        printf("cJSON_Print result:\n%s\n", out);
        printf("cJSON_PrintPreallocated result:\n%s\n", buf_fail);
        free(out);
        free(buf_fail);
        free(buf);
        return -1;
    }

    free(out);
    free(buf_fail);
    free(buf);
    return 0;
}

/* Create a bunch of objects as demonstration. */
static void create_objects(void)
{
    /* declare a few. */
    cJSON *root = NULL;
    cJSON *fmt = NULL;
    cJSON *img = NULL;
    cJSON *thm = NULL;
    cJSON *fld = NULL;
    int i = 0;

    /* Our "days of the week" array: */
    const char *strings[7] =
    {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };
    /* Our matrix: */
    int numbers[3][3] =
    {
        {0, -1, 0},
        {1, 0, 0},
        {0 ,0, 1}
    };
    /* Our "gallery" item: */
    int ids[4] = { 116, 943, 234, 38793 };
    /* Our array of "records": */
    struct record fields[2] =
    {
        {
            "zip",
            37.7668,
            -1.223959e+2,
            "",
            "SAN FRANCISCO",
            "CA",
            "94107",
            "US"
        },
        {
            "zip",
            37.371991,
            -1.22026e+2,
            "",
            "SUNNYVALE",
            "CA",
            "94085",
            "US"
        }
    };
    volatile double zero = 0.0;

    /* Here we construct some JSON standards, from the JSON site. */

    /* Our "Video" datatype: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "name", cJSON_CreateString("Jack (\"Bee\") Nimble"));
    cJSON_AddItemToObject(root, "format", fmt = cJSON_CreateObject());
    cJSON_AddStringToObject(fmt, "type", "rect");
    cJSON_AddNumberToObject(fmt, "width", 1920);
    cJSON_AddNumberToObject(fmt, "height", 1080);
    cJSON_AddFalseToObject (fmt, "interlace");
    cJSON_AddNumberToObject(fmt, "frame rate", 24);

    /* Print to text */
    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our "days of the week" array: */
    root = cJSON_CreateStringArray(strings, 7);

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our matrix: */
    root = cJSON_CreateArray();
    for (i = 0; i < 3; i++)
    {
        cJSON_AddItemToArray(root, cJSON_CreateIntArray(numbers[i], 3));
    }

    /* cJSON_ReplaceItemInArray(root, 1, cJSON_CreateString("Replacement")); */

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our "gallery" item: */
    root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "Image", img = cJSON_CreateObject());
    cJSON_AddNumberToObject(img, "Width", 800);
    cJSON_AddNumberToObject(img, "Height", 600);
    cJSON_AddStringToObject(img, "Title", "View from 15th Floor");
    cJSON_AddItemToObject(img, "Thumbnail", thm = cJSON_CreateObject());
    cJSON_AddStringToObject(thm, "Url", "http:/*www.example.com/image/481989943");
    cJSON_AddNumberToObject(thm, "Height", 125);
    cJSON_AddStringToObject(thm, "Width", "100");
    cJSON_AddItemToObject(img, "IDs", cJSON_CreateIntArray(ids, 4));

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    /* Our array of "records": */
    root = cJSON_CreateArray();
    for (i = 0; i < 2; i++)
    {
        cJSON_AddItemToArray(root, fld = cJSON_CreateObject());
        cJSON_AddStringToObject(fld, "precision", fields[i].precision);
        cJSON_AddNumberToObject(fld, "Latitude", fields[i].lat);
        cJSON_AddNumberToObject(fld, "Longitude", fields[i].lon);
        cJSON_AddStringToObject(fld, "Address", fields[i].address);
        cJSON_AddStringToObject(fld, "City", fields[i].city);
        cJSON_AddStringToObject(fld, "State", fields[i].state);
        cJSON_AddStringToObject(fld, "Zip", fields[i].zip);
        cJSON_AddStringToObject(fld, "Country", fields[i].country);
    }

    /* cJSON_ReplaceItemInObject(cJSON_GetArrayItem(root, 1), "City", cJSON_CreateIntArray(ids, 4)); */

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);

    root = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "number", 1.0 / zero);

    if (print_preallocated(root) != 0) {
        cJSON_Delete(root);
        exit(EXIT_FAILURE);
    }
    cJSON_Delete(root);
}

int main(void)
{
    /* a bunch of json: */
    char text1[] =
        "{\n"
        "\"name\": \"Jack (\\\"Bee\\\") Nimble\", \n"
        "\"format\": {\"type\":       \"rect\", \n"
        "\"width\":      1920, \n"
        "\"height\":     1080, \n"
        "\"interlace\":  false,\"frame rate\": 24\n"
        "}\n"
        "}";
    char text2[] = "[\"Sunday\", \"Monday\", \"Tuesday\", \"Wednesday\", \"Thursday\", \"Friday\", \"Saturday\"]";
    char text3[] =
        "[\n"
        "    [0, -1, 0],\n"
        "    [1, 0, 0],\n"
        "    [0, 0, 1]\n"
        "\t]\n";
    char text4[] =
        "{\n"
        "\t\t\"Image\": {\n"
        "\t\t\t\"Width\":  800,\n"
        "\t\t\t\"Height\": 600,\n"
        "\t\t\t\"Title\":  \"View from 15th Floor\",\n"
        "\t\t\t\"Thumbnail\": {\n"
        "\t\t\t\t\"Url\":    \"http:/*www.example.com/image/481989943\",\n"
        "\t\t\t\t\"Height\": 125,\n"
        "\t\t\t\t\"Width\":  \"100\"\n"
        "\t\t\t},\n"
        "\t\t\t\"IDs\": [116, 943, 234, 38793]\n"
        "\t\t}\n"
        "\t}";
    char text5[] =
        "[\n"
        "\t {\n"
        "\t \"precision\": \"zip\",\n"
        "\t \"Latitude\":  37.7668,\n"
        "\t \"Longitude\": -122.3959,\n"
        "\t \"Address\":   \"\",\n"
        "\t \"City\":      \"SAN FRANCISCO\",\n"
        "\t \"State\":     \"CA\",\n"
        "\t \"Zip\":       \"94107\",\n"
        "\t \"Country\":   \"US\"\n"
        "\t },\n"
        "\t {\n"
        "\t \"precision\": \"zip\",\n"
        "\t \"Latitude\":  37.371991,\n"
        "\t \"Longitude\": -122.026020,\n"
        "\t \"Address\":   \"\",\n"
        "\t \"City\":      \"SUNNYVALE\",\n"
        "\t \"State\":     \"CA\",\n"
        "\t \"Zip\":       \"94085\",\n"
        "\t \"Country\":   \"US\"\n"
        "\t }\n"
        "\t ]";

    char text6[] =
        "<!DOCTYPE html>"
        "<html>\n"
        "<head>\n"
        "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
        "  <style type=\"text/css\">\n"
        "    html, body, iframe { margin: 0; padding: 0; height: 100%; }\n"
        "    iframe { display: block; width: 100%; border: none; }\n"
        "  </style>\n"
        "<title>Application Error</title>\n"
        "</head>\n"
        "<body>\n"
        "  <iframe src=\"//s3.amazonaws.com/heroku_pages/error.html\">\n"
        "    <p>Application Error</p>\n"
        "  </iframe>\n"
        "</body>\n"
        "</html>\n";

    /* Process each json textblock by parsing, then rebuilding: */
    doit(text1);
    doit(text2);
    doit(text3);
    doit(text4);
    doit(text5);
    doit(text6);

    /* Parse standard testfiles: */
    /* dofile("../../tests/test1"); */
    /* dofile("../../tests/test2"); */
    /* dofile("../../tests/test3"); */
    /* dofile("../../tests/test4"); */
    /* dofile("../../tests/test5"); */
    /* dofile("../../tests/test6"); */

    /* Now some samplecode for building objects concisely: */
    create_objects();

    return 0;
}
