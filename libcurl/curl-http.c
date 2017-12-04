/**
 * file: curl-http.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "curl/curl.h"
#include "log.h"


/* 保存下载文件 */
static size_t wirtefunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	return fwrite(ptr, size, nmemb, stream);
}

/**
 * 下载文件
 * url: 网络地址
 * local: 本地路径
 * timeout: 连接超时(秒)
 */
int http_download(const char *url, const char *local, long timeout)
{
	CURL *curlhandle = NULL;
	FILE *fd;
	CURLcode ret;

	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();
	if (NULL == curlhandle) {
		LOG_ERR("curl_easy_init failed!\n");
		return -1;
	}

	fd = fopen(local, "w+");
	if (fd < 0) {
		LOG_ERR("create %s failed\n", local);
		goto err_1;
	}

	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout); //设置连接超时, 单位秒
	curl_easy_setopt(curlhandle, CURLOPT_WRITEDATA, fd);
	curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, wirtefunc);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(curlhandle);
	if (ret != CURLE_OK) {
		LOG_DEBUG("curl_easy_perform failed: %s\n", curl_easy_strerror(ret));
		goto err_2;
	}

	fclose(fd);
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();

	return 0;

err_2:
	fclose(fd);
err_1:
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();
	return -1;
}


#if 1
/* 读取上传文件 */
static size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
	FILE *f = stream;
	size_t ret;
	curl_off_t nread;

	if (ferror(f))
		return CURL_READFUNC_ABORT;

	ret = fread(ptr, size, nmemb, f);
	nread = (curl_off_t)ret;

	LOG_DEBUG("read %"CURL_FORMAT_CURL_OFF_T" bytes from file\n", nread);

	return ret;
}

/**
 * 上传文件
 */
int http_upload(const char *url, const char *local, long timeout)
{
	CURL *curlhandle = NULL;
	FILE *fd;
	CURLcode ret;
	struct stat file_info;

	/* get the file size of the local file */
	stat(local, &file_info);

	fd = fopen(local, "rb");
	if (fd < 0) {
		LOG_ERR("create %s failed\n", local);
		return -1;
	}

	curl_global_init(CURL_GLOBAL_ALL);
	curlhandle = curl_easy_init();
	if (NULL == curlhandle) {
		LOG_ERR("curl_easy_init failed!\n");
		goto err_1;
	}

	curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_PUT, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_URL, url);
	curl_easy_setopt(curlhandle, CURLOPT_CONNECTTIMEOUT, timeout); //设置连接超时, 单位秒
	curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
	curl_easy_setopt(curlhandle, CURLOPT_READDATA, fd);
	curl_easy_setopt(curlhandle, CURLOPT_INFILESIZE_LARGE, (curl_off_t)file_info.st_size);
	curl_easy_setopt(curlhandle, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);

	ret = curl_easy_perform(curlhandle);
	if (ret != CURLE_OK) {
		LOG_DEBUG("curl_easy_perform failed: %s\n", curl_easy_strerror(ret));
		goto err_2;
	}

	fclose(fd);
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();

	return 0;

err_2:
	curl_easy_cleanup(curlhandle);
	curl_global_cleanup();
err_1:
	fclose(fd);
	return -1;
}
#endif

#if 1
int main(int argc, char **argv)
{
	if (argc != 4) {
		printf("Usage: %s <url> <local> <r/w>\n", argv[0]);
		exit(1);
	}

	if (strcmp(argv[3], "r")==0) {
	if (0 == http_download(argv[1], argv[2], 10)) {
		printf("download %s OK\n", argv[2]);
	} else {
		printf("download %s failed\n", argv[2]);
	}
	}
	else if (strcmp(argv[3], "w")==0) {
		http_upload(argv[1], argv[2], 10);
	}

	return 0;
}
#endif

