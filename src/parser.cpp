#include "parser.hpp"

#include <nfd.h>

#include <TFile.h>
#include <TGraph.h>
#include <TH1.h>
#include <TKey.h>
#include <TList.h>

File::File(const std::string& path)
    : path(path)
{
    auto it = path.find_last_of('\\');

    if (it != std::string::npos) {
        name = path.substr(it + 1);
    }

    else {
        name = path;
    }
}

File File::open() {
    nfdu8char_t* path;

    nfdopendialogu8args_t flags = {};
    nfdu8filteritem_t filters[] = { { "Root file", "root" } };
    flags.filterList = filters;
    flags.filterCount = sizeof(filters) / sizeof(nfdu8filteritem_t);

    nfdresult_t res = NFD_OpenDialogU8_With(&path, &flags);

    if (res != NFD_OKAY || path == nullptr) {
        return File("");
    }

    return File(path);
}

const std::string& File::getName() const {
    return name;
}

const std::string& File::getPath() const {
    return path;
}

std::vector<Data>& File::getData() {
    return data;
}

void Parser::parse(File& file) {
    TFile* f = TFile::Open(file.getPath().c_str());

    TList* keys = f->GetListOfKeys();
    TIter nextKey(keys);
    TKey* key;

    while (key = (TKey*)nextKey()) {
        std::string className = key->GetClassName();

        if (className == "TGraph") {
            TGraph* graph = dynamic_cast<TGraph*>(key->ReadObj());

            if (graph) {
                Data data;

                data.cname = className;
                data.name = graph->GetName();

                double* xPoints = graph->GetX();
                double* yPoints = graph->GetY();

                for (int i = 0; i < graph->GetN(); ++i) {
                    data.x.push_back(xPoints[i]);
                    data.y.push_back(yPoints[i]);
                }

                file.getData().push_back(data);
            }
        }

        else if (className == "TH1F") {
            TH1F* histogram = dynamic_cast<TH1F*>(key->ReadObj());

            if (histogram) {
                Data data;

                data.cname = className;
                data.name = histogram->GetName();

                for (int i = 1; i <= histogram->GetNbinsX(); ++i) {
                    data.x.push_back(histogram->GetBinContent(i));
                    data.y.push_back(histogram->GetBinCenter(i));
                    data.bins = histogram->GetNbinsX();
                }

                file.getData().push_back(data);
            }
        }
    }

    f->Close();
    delete f;
}

std::string File::save() {
    nfdu8char_t* path = nullptr;

    nfdsavedialogu8args_t flags = {};
    nfdu8filteritem_t filters[] = { { "PNG Image", "png" } };
    flags.filterList = filters;
    flags.filterCount = sizeof(filters) / sizeof(nfdu8filteritem_t);

    nfdresult_t dialog = NFD_SaveDialogU8_With(&path, &flags);

    if (dialog != NFD_OKAY || path == nullptr) {
        return "";
    }

    std::string res(path);

    if (res.size() < 4 || res.substr(res.size() - 4) != ".png") {
        res += ".png";
    }

    NFD_FreePathU8(path);
    return res;
}