#include "parser.hpp"

#include <nfd.h>

#include <TFile.h>
#include <TGraph.h>
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
    }

    f->Close();
    delete f;
}