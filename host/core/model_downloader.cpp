#include <stdio.h>
#include <curl/curl.h>
#include <string>
#include <iostream>
#include <cmath>
#include <sstream>
#include <fstream>

#define BLUE "\033[94m"
#define ENDC "\033[0m"
#define RED "\033[91m"

#define MAX_SHAVES (16)
#define MAX_CMX_SLICES (16)
#define MAX_NCES (2)

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

static int progress_func(void *ptr, double TotalToDownload, double NowDownloaded,
                         double TotalToUpload, double NowUploaded)
{
    int progress_bar_width = 50;
    double download_progress = TotalToDownload ? (NowDownloaded / TotalToDownload) : 0;
    int progress_indicator_width = download_progress * progress_bar_width;

    int p_bar_i = 0;
    printf("%3.0f%% ", download_progress * 100);
    printf(RED "[" ENDC);
    printf(BLUE);
    for (; p_bar_i < progress_indicator_width; p_bar_i++)
    {
        printf("-");
    }
    printf(ENDC);
    for (; p_bar_i < progress_bar_width; p_bar_i++)
    {
        printf(" ");
    }
    printf(RED "]" ENDC "\r");
    fflush(stdout);
    return 0;
}

static int check_input(int nr_shaves, int nr_cmx_slices, int nr_NCEs)
{
    if (nr_shaves <= 0 || nr_shaves > MAX_SHAVES)
    {
        std::cerr << RED "Number of shaves must be between (0, 16]" << ENDC << std::endl;
        return 1;
    }
    if (nr_cmx_slices <= 0 || nr_cmx_slices > MAX_CMX_SLICES)
    {
        std::cerr << RED "Number of shaves must be between (0, 16]" << ENDC << std::endl;
        return 2;
    }
    if (nr_NCEs < 0 || nr_NCEs > MAX_NCES)
    {
        std::cerr << RED "Number of shaves must be between [0, 2]" << ENDC << std::endl;
        return 3;
    }
    return 0;
}

int download_model(std::string model_name, int nr_shaves, int nr_cmx_slices, int nr_NCEs, std::string output_folder_path)
{
    if (check_input(nr_shaves, nr_cmx_slices, nr_NCEs))
        return 3;
    CURL *hnd = curl_easy_init();

    if (0)
        curl_easy_setopt(hnd, CURLOPT_VERBOSE, 1);

    curl_easy_setopt(hnd, CURLOPT_USERAGENT, "Mozilla/5.0 (Windows; U; Windows NT 5.1; en-US) AppleWebKit/525.13 (KHTML, like Gecko) Chrome/0.A.B.C Safari/525.13");
    curl_easy_setopt(hnd, CURLOPT_CUSTOMREQUEST, "POST");
    curl_easy_setopt(hnd, CURLOPT_URL, "http://luxonis.com:8080/");

    std::string readBuffer;

    curl_easy_setopt(hnd, CURLOPT_TIMEOUT, 120L);
    curl_easy_setopt(hnd, CURLOPT_CONNECTTIMEOUT, 10L);

    struct curl_slist *headers = NULL;
    // headers = curl_slist_append(headers, "postman-token: 5824015d-b523-e286-69e0-9a4da23af7da");
    // headers = curl_slist_append(headers, "cache-control: no-cache");
    headers = curl_slist_append(headers, "content-type: multipart/form-data; boundary=----WebKitFormBoundary7MA4YWxkTrZu0gW");
    curl_easy_setopt(hnd, CURLOPT_HTTPHEADER, headers);

    curl_easy_setopt(hnd, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(hnd, CURLOPT_WRITEDATA, &readBuffer);

    // Internal CURL progressmeter must be disabled if we provide our own callback
    curl_easy_setopt(hnd, CURLOPT_NOPROGRESS, 0);
    // Install the callback function
    curl_easy_setopt(hnd, CURLOPT_PROGRESSFUNCTION, progress_func);

    std::string boundary = "------WebKitFormBoundary7MA4YWxkTrZu0gW";
    std::string conent_header = "Content-Disposition: form-data; ";
    std::string end_form_line = "\r\n";
    std::string end_form = end_form_line + end_form_line;

    std::ostringstream os;

    std::string platform = nr_NCEs == 0 ? "VPU_MYRIAD_2450" : "VPU_MYRIAD_2480";

    os << boundary << end_form_line;
    os << conent_header << "name=\"compile_type\"" << end_form << "zoo\r\n";
    os << boundary << end_form_line;
    os << conent_header << "name=\"model_name\"" << end_form << model_name << end_form_line;
    os << boundary << end_form_line;
    os << conent_header << "name=\"model_downloader_params\"" << end_form << "--precisions FP16 --num_attempts 5\r\n";
    os << boundary << end_form_line;
    os << conent_header << "name=\"intermediate_compiler_params\"" << end_form << "--data_type=FP16 --mean_values [127.5,127.5,127.5] --scale_values [255,255,255]\r\n";
    os << boundary << end_form_line;
    os << conent_header << "name=\"compiler_params\"" << end_form << "-ip U8 -VPU_MYRIAD_PLATFORM " << platform << " -VPU_NUMBER_OF_SHAVES " << nr_shaves << " -VPU_NUMBER_OF_CMX_SLICES " << nr_cmx_slices << end_form_line;
    os << boundary << "--";

    std::string options_builder = os.str();
    // std::cout << options_builder << std::endl;

    curl_easy_setopt(hnd, CURLOPT_POSTFIELDS, options_builder.c_str());

    CURLcode ret = curl_easy_perform(hnd);
    // std::cout << ret << std::endl;
    std::cout << std::endl;

    int ret_code = 0;

    long http_code = 0;
    curl_easy_getinfo(hnd, CURLINFO_RESPONSE_CODE, &http_code);
    if (http_code == 200 && ret == CURLE_OK)
    {
        //Succeeded
        std::cout << "Model downloaded!" << std::endl;
        // std::cout << readBuffer.size() << std::endl;
        ret_code = 0;
        std::ofstream wf(output_folder_path, std::ios::out | std::ios::binary);
        if (!wf)
        {
            std::cerr << "Cannot open file!" << std::endl;
            ret_code = 1;
            goto cleanup;
        }

        wf.write(readBuffer.c_str(), readBuffer.size());
        wf.close();
        if (!wf.good())
        {
            std::cerr << "Error occurred at writing time!" << std::endl;
            ret_code = 2;
            goto cleanup;
        }
    }
    else
    {
        //Failed
        std::cerr << RED "Model download failed with http code : " << http_code << ", curl code: " << ret << ENDC << std::endl;
        std::cerr << readBuffer << std::endl;
        ret_code = http_code;
    }

cleanup:
    curl_easy_cleanup(hnd);

    return (int)ret_code;
}

// int main(void)
// {
//     std::string model_name = "mobilenet-ssd";
//     int nr_shaves = 6;
//     int nr_cmx_slices = 6;
//     int nr_NCEs = 0;
//     std::string output_path = ".";
//     std::ostringstream path;
//     path << output_path << "/" << model_name << ".blob" << ".sh" << nr_shaves << "cmx" << nr_cmx_slices;
//     if(nr_NCEs == 0)
//     {
//         path << "NO_NCE";
//     }
//     return download_model(model_name, nr_shaves, nr_cmx_slices, nr_NCEs, path.str());
// }