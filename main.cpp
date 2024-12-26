#include <iostream>
#include <curl/curl.h>
#include <json/json.h>

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "http://api.openweathermap.org/data/2.5/weather?q=London&appid=YOUR_API_KEY");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);

        if(res != CURLE_OK) {
            std::cerr << "cURL error: " << curl_easy_strerror(res) << std::endl;
            return 1;
        }

        Json::CharReaderBuilder readerBuilder;
        Json::Value root;
        std::string errors;

        std::istringstream stream(readBuffer);
        if (!Json::parseFromStream(readerBuilder, stream, &root, &errors)) {
            std::cerr << "Failed to parse JSON: " << errors << std::endl;
            return 1;
        }

        std::cout << "Weather in " << root["name"].asString() << ": " << root["weather"][0]["description"].asString() << std::endl;
        std::cout << "Temperature: " << root["main"]["temp"].asFloat() - 273.15 << " °C" << std::endl;
    }

    return 0;
}
