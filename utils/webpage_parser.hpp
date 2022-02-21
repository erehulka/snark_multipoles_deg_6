#include <graphs.hpp>
#include "../implementation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <curl/curl.h>

using namespace ba_graph;

/**
 * Function copied from stackoverflow
 * https://stackoverflow.com/questions/9786150/save-curl-content-result-into-a-string-in-c
 */
static size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string request_html_page(const std::string& input_url) {
    CURL *curl;
    CURLcode res;
    std::string read_buffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, input_url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &read_buffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }
    return read_buffer;
}

std::vector<std::string> split_string(const std::string& s){
    std::vector<std::string> v;
    std::string temp;

    for (char i : s){
        if(i=='\n'){
            if (!temp.empty()) v.push_back(temp);
            temp = "";
        }
        else{
            temp.push_back(i);
        }
    }
    if (!temp.empty()) v.push_back(temp);

    return v;
}

std::vector<Graph> generate_graphs_from_webpage(const std::string& input_url) {
    std::vector<Graph> result;

    std::string content = request_html_page(input_url);
    std::vector<std::string> graph_codes = split_string(content);

    // TODO: Paralelne
    for (const std::string& graph_repr : graph_codes) {
        result.emplace_back(read_graph6_line(graph_repr));
    }

    return result;
}
