#include "application.hpp"
#include "container.hpp"
#include "text.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "browser.hpp"

int main() {
    Application app(Rect::percent(0, 0, 90, 80), "Root Batch Viewer");

    std::vector<float> x;
    std::vector<float> y;
    for (float i = 0; i <= 100000; i += 1) {
        x.push_back(i);
        y.push_back(i);
    }

    std::vector<std::shared_ptr<Plot>> p;
    for (int i = 0; i < 20; ++i) {
        p.push_back(app.add<Plot>(Rect::percent(1, 6, 98, 88), "Plot " + std::to_string(i + 1)));
        p[i]->setTab(2);
        p[i]->setVisible(false);
        p[i]->setData(x, y);
    }
    int curr = 0;
    p[curr]->setVisible(true);

    std::vector<float> hx;
    float mean = 10.0f;
    float sigma = 3.0f;

    for (int i = 0; i < 100; ++i) {
        float exponent = -std::pow(i - mean, 2) / (2 * std::pow(sigma, 2));
        float weight = std::exp(exponent);

        int count = static_cast<int>(weight * 100);

        for (int j = 0; j < count; ++j) {
            hx.push_back((float)i);
        }
    }

    std::vector<std::shared_ptr<Histogram>> h;
    for (int i = 0; i < 20; ++i) {
        h.push_back(app.add<Histogram>(Rect::percent(1, 6, 98, 88), "Histogram " + std::to_string(i + 1)));
        h[i]->setTab(2);
        h[i]->setVisible(false);
        h[i]->setData(hx, x);
        h[i]->setBins(-3);
    }
    int currH = 0;

    auto prop = app.add<Container>(Rect::percent(1, 1, 98, 4), "Properties");
    prop->setTab(2);

    auto ps = prop->add<ToggleButton>(Rect::percent(0, 0, 10, 100), "Plots");
    ps->setToggle(true);

    auto hs = prop->add<ToggleButton>(Rect::percent(10, 0, 10, 100), "Histograms");

    auto b1 = prop->add<ToggleButton>(Rect::percent(70, 0, 10, 100), "Log X");
    b1->setCallback(
        [&p, &curr] {
            p[curr]->setLogX(!p[curr]->getLogX());
            p[curr]->reset();
        }
    );

    auto b2 = prop->add<ToggleButton>(Rect::percent(80, 0, 10, 100), "Log Y");
    b2->setCallback(
        [&p, &curr] {
            p[curr]->setLogY(!p[curr]->getLogY());
            p[curr]->reset();
        }
    );

    auto sync = [&p, &curr, &b1, &b2] {
        b1->setToggle(p[curr]->getLogX());
        b2->setToggle(p[curr]->getLogY());
    };

    auto b3 = prop->add<Button>(Rect::percent(90, 0, 10, 100), "Save");

    auto nav = app.add<Container>(Rect::percent(1, 95, 98, 4), "Navigation");
    nav->setTab(2);

    auto t2 = nav->add<Input>(Rect::percent(45, 0, 10, 100), "Page");
    t2->setVal(1);
    t2->setCallback(
        [&p, &curr, &t2, &sync] {
            int newPage = t2->getVal();

            if (newPage >= 1 && newPage <= static_cast<int>(p.size())) {
                int newIndex = newPage - 1;

                if (newIndex != curr) {
                    p[curr]->setVisible(false);

                    curr = newIndex;
                    p[curr]->setVisible(true);

                    sync();
                }
            }

            else {
                t2->setVal(curr + 1);
            }
        }
    );

    auto b4 = nav->add<Button>(Rect::percent(0, 0, 45, 100), "Prev");
    b4->setCallback(
        [&] {
            if (ps->getToggle()) {
                if (curr > 0) {
                    p[curr]->setVisible(false);
                    p[--curr]->setVisible(true);

                    t2->setVal(curr + 1);

                    sync();
                }
            }

            else if (hs->getToggle()) {
                if (currH > 0) {
                    h[currH]->setVisible(false);
                    h[--currH]->setVisible(true);

                    t2->setVal(currH + 1);

                    sync();
                }
            }
        }
    );

    auto b5 = nav->add<Button>(Rect::percent(55, 0, 45, 100), "Next");
    b5->setCallback(
        [&] {
            if (ps->getToggle()) {
                if (curr < p.size() - 1) {
                    p[curr]->setVisible(false);
                    p[++curr]->setVisible(true);

                    t2->setVal(curr + 1);

                    sync();
                }
            }

            else if (hs->getToggle()) {
                if (currH < h.size() - 1) {
                    h[currH]->setVisible(false);
                    h[++currH]->setVisible(true);

                    t2->setVal(currH + 1);

                    sync();
                }
            }
        }
    );

    auto c1 = app.add<Container>(Rect::percent(1, 1, 98, 98), "Root");
    c1->setTab(0);

    auto c2 = c1->add<Container>(Rect::percent(0, 0, 100, 91), "Browser");
    c2->setColor(1, 1, 1, 1);

    auto fs = c2->add<Browser>(Rect::percent(0, 0, 100, 100), "Files");
    // fs->setElementRect(Rect::percent(0, 0, 100, 4));

    auto b6 = c1->add<Button>(Rect::percent(0, 96, 100, 4), "Open File");
    b6->setCallback(
        [&fs] {
            fs->add(File::open());
        }
    );

    auto b7 = c1->add<Button>(Rect::percent(0, 92, 100, 4), "Link");
    b7->setCallback(
        [&] {
            Data& data = fs->getData();

            if (data.cname == "TGraph") {
                p[curr]->setData(data.x, data.y);
                p[curr]->reset();
            }

            else if (data.cname == "TH1F") {
                h[currH]->setData(data.x, data.y);
                h[currH]->setBins(data.bins);
                h[currH]->reset();
            }
        }
    );

    ps->setCallback(
        [&] {
            hs->setToggle(!hs->getToggle());

            b1->setVisible(true);
            b2->setVisible(true);

            p[curr]->setVisible(true);
            h[currH]->setVisible(false);

            t2->setVal(curr + 1);
        }
    );

    hs->setCallback(
        [&] {
            ps->setToggle(!ps->getToggle());

            b1->setVisible(false);
            b2->setVisible(false);

            p[curr]->setVisible(false);
            h[currH]->setVisible(true);

            t2->setVal(currH + 1);
        }
    );

    app.run();
}