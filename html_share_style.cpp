#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIE(msg, ...) do { fprintf(stderr, msg, ##__VA_ARGS__); exit(EXIT_FAILURE); } while(0)

struct res_html {
    char*  html;
    size_t size;
};


struct res_html load_html_file(const char* filename)
{
    FILE* fh = fopen(filename, "rb");
    if (fh == NULL) {
        DIE("Can't open html file: %s\n", filename);
    }

    if (fseek(fh, 0L, SEEK_END) != 0) {
        DIE("Can't set position (fseek) in file: %s\n", filename);
    }

    long size = ftell(fh);

    if (fseek(fh, 0L, SEEK_SET) != 0) {
        DIE("Can't set position (fseek) in file: %s\n", filename);
    }

    if (size <= 0) {
        fclose(fh);

        struct res_html res = {NULL, 0};
        return res;
    }

    char* html = new char[size + 1];
    if (html == NULL) {
        DIE("Can't allocate mem for html file: %s\n", filename);
    }

    size_t nread = fread(html, 1, size, fh);
    if (nread != size || nread == 0) {
        DIE("could not read %ld bytes\n", size);
    }

    fclose(fh);

    struct res_html res = {html, (size_t)size};
    return res;
}

// 支持大小写字符窜查找
char*  stristr( const char* str1, const char* str2 )
{
    char* cp = (char*) str1;
    char* s1, *s2;

    if (!*str2)
        return ((char*)str1);

    while (*cp) {
        s1 = cp;
        s2 = (char*) str2;

        while (*s1 && *s2 && (!(*s1 - *s2) || !(*s1 - *s2 - 32) || !(*s1 - *s2 + 32))) {
            s1++, s2++;
        }

        if (!*s2)
            return (cp);

        cp++;
    }

    return (NULL);

}

// 查找HTML文件 head_body 标签
char* find_head_body(const char* str)
{
    char* pch;
    pch = stristr(str, "</head>");
    if (pch == NULL) {
        DIE("找到不到html标签!\n");
    }

    pch = stristr(pch, "<body");
    if (pch == NULL) {
        DIE("找到不到html标签!\n");
    }

    pch = strchr(pch, '>') ;
    if (pch == NULL) {
        DIE("找到不到html标签!\n");
    }

    return pch + 1;
}

static void usage(void)
{
    fprintf(stderr, "Usage: html_share_style  <file.html>  <from.html>\n");
}

int main(int argc, char* argv[])
{
    if (argc != 3) {
        usage();
        DIE("Invalid number of arguments\n");
    }

    // 加载 2个html文件
    struct res_html file = load_html_file(argv[1]);
    struct res_html from = load_html_file(argv[2]);

    if ((from.size == 0) || (file.size == 0)) {
        DIE("空文件!\n");
    }

    // 查找HTML文件 head_body 标签
    char* p_head;
    p_head = find_head_body(from.html);

    char* p_body;
    p_body = find_head_body(file.html);


    // 测试效果
//    *p_head = '\0';
//    printf("%s", from.html);
//    printf("%s", p_body);


    // 保存文件
    const char* filename = argv[1];
    FILE* fh = fopen(filename, "wb");
    if (fh == NULL) {
        DIE("Can't open html file: %s\n", filename);
    }

    // 组合html文件
    fwrite(from.html, 1, p_head - from.html, fh);
    fwrite(p_body, 1, file.size - (p_body - file.html), fh);

    fclose(fh);


    delete[] from.html;
    delete[] file.html;

    return 0;
}
