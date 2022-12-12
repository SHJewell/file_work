// file_work.cpp
// compile by using: /EHsc /W4 /permissive- /std:c++17 (or later)
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <filesystem>
#include <sys/stat.h>
#include <vector>

// using namespace std;
using namespace std::filesystem;


class fileInformation
{
public:
    int numFiles;
    std::vector<std::string> filePaths;
    std::vector<std::string> fileNames;
    std::vector<std::string> containingFolder;
    std::vector<std::string> extension;
    path pathToCrawl = "";

public:
    std::wstring DisplayPathInfo(path pathToDisplay)
    {
        // This path may or may not refer to an existing file. We are
        // examining this path string, not file system objects.
        // path pathToDisplay(L"E:\Documents\Datasets\Weather Data\Copernicus ");

        std::wostringstream wos;
        int i = 0;
        wos << L"Displaying path info for: " << pathToDisplay << std::endl;
        for (path::iterator itr = pathToDisplay.begin(); itr != pathToDisplay.end(); ++itr)
        {
            wos << L"path part: " << i++ << L" = " << *itr << std::endl;
        }

        wos << L"root_name() = " << pathToDisplay.root_name() << std::endl
            << L"root_path() = " << pathToDisplay.root_path() << std::endl
            << L"relative_path() = " << pathToDisplay.relative_path() << std::endl
            << L"parent_path() = " << pathToDisplay.parent_path() << std::endl
            << L"filename() = " << pathToDisplay.filename() << std::endl
            << L"stem() = " << pathToDisplay.stem() << std::endl
            << L"extension() = " << pathToDisplay.extension() << std::endl;

        return wos.str();
    }

public:
    void iterator_test(path curPath) {

        for (const auto& entry : directory_iterator(curPath)) {

            // std::cout << entry.path() << std::endl;

            std::error_code ec;

            if (is_directory(entry.path(), ec))
            {
                path newPath = entry.path();
                iterator_test(newPath);
            }
            else if (is_regular_file(entry.path(), ec))
            {
                std::size_t found = entry.path().string().find_last_of("/\\");
                std::size_t second_to_last = entry.path().string().substr(0, found).find_last_of("/\\");
                std::size_t ext_loc = entry.path().string().find_first_of(".");

                numFiles += 1;
                filePaths.push_back(entry.path().string());
                fileNames.push_back(entry.path().string().substr(found + 1));
                containingFolder.push_back(entry.path().string().substr(second_to_last + 1, (found - second_to_last - 1)));
                extension.push_back(entry.path().string().substr(ext_loc));

                /*
                std::cout << "path: " << entry.path().string().substr(0, found) << std::endl;
                std::cout << "file: " << entry.path().string().substr(found+1) << std::endl;
                */
            }
        }

    }

public:
    void crawlPath() {
        if (pathToCrawl == "")
        {
            std::cout << "No path defined!" << std::endl;
            return;
        }

        iterator_test(pathToCrawl);
    }

public:
    void write_file_list(std::string outputName) {

        std::ofstream myfile;
        myfile.open(outputName);

        myfile << "path,folder,name,extension\n";

        for (int i = 0; i < numFiles; i++) {
            myfile << filePaths[i] << ",";
            myfile << containingFolder[i] << ",";
            myfile << fileNames[i] << ",";
            myfile << extension[i] << ",";
            myfile << "\n";
        }

        myfile.close();

        std::cout << "File written to: " << outputName << std::endl;

    }

};


int main() {

    fileInformation file_dir;

    file_dir.pathToCrawl = L"E:\\Documents\\Datasets\\Weather Data\\Copernicus";

    file_dir.crawlPath();
    
    file_dir.write_file_list("E:\\Documents\\Coding\\wait_five_minutes_weather\\nc_file_directory.csv");
    
    //wcout << DisplayPathInfo(L"E:\Documents\Datasets\Weather Data\Copernicus ") << endl;
    // wcout << ComparePaths() << endl; // see following example
    // wcout << endl << L"Press Enter to exit" << endl;
    // wstring input;
    // getline(wcin, input);
}
