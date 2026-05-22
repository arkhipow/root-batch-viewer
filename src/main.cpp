#include "application.hpp"
#include "container.hpp"
#include "text.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "browser.hpp"

#include <iostream>

int main() {
    Application app(Rect::percent(0, 0, 90, 80), "Root Batch Viewer");

    std::vector<float> x;
    std::vector<float> y;
    for (float i = 0; i <= 100; i += 1) {
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

    auto b1 = prop->add<ToggleButton>(Rect::percent(60, 0, 10, 100), "Log X");
    b1->setCallback(
        [&p, &curr] {
            p[curr]->setLogX(!p[curr]->getLogX());
            p[curr]->reset();
        }
    );

    auto b2 = prop->add<ToggleButton>(Rect::percent(70, 0, 10, 100), "Log Y");
    b2->setCallback(
        [&p, &curr] {
            p[curr]->setLogY(!p[curr]->getLogY());
            p[curr]->reset();
        }
    );

    auto m = prop->add<ToggleButton>(Rect::percent(80, 0, 10, 100), "Measure");
    m->setCallback(
        [&] {
            if (ps->getToggle()) {
                p[curr]->setMeasure(!p[curr]->getMeasure());
            }

            else if (hs->getToggle()) {
                h[currH]->setMeasure(!h[currH]->getMeasure());
            }
        }
    );

    auto tl = app.add<InputText>(Rect::percent(1, 1, 10, 4), "Title");
    tl->setText(p[curr]->getLable());

    auto ax = app.add<InputText>(Rect::percent(1, 6, 10, 4), "Axis X");
    ax->setTab(3);
    ax->setText(p[curr]->getAxisX());

    auto ay = app.add<InputText>(Rect::percent(1, 11, 10, 4), "Axis Y");
    ay->setTab(3);
    ay->setText(p[curr]->getAxisY());

    auto sync = [&] {
        if (ps->getToggle()) {
            b1->setToggle(p[curr]->getLogX());
            b2->setToggle(p[curr]->getLogY());
            m->setToggle(p[curr]->getMeasure());
            tl->setText(p[curr]->getLable());
            ax->setText(p[curr]->getAxisX());
            ay->setText(p[curr]->getAxisY());
        }

        else if (hs->getToggle()) {
            m->setToggle(h[currH]->getMeasure());
            tl->setText(h[currH]->getLable());
            ax->setText(h[currH]->getAxisX());
            ay->setText(h[currH]->getAxisY());
        }
    };

    auto b3 = prop->add<Button>(Rect::percent(90, 0, 10, 100), "Save");

    auto nav = app.add<Container>(Rect::percent(1, 95, 98, 4), "Navigation");
    nav->setTab(2);

    auto t2 = nav->add<Input>(Rect::percent(45, 0, 10, 100), "Page");
    t2->setVal(1);
    t2->setCallback(
        [&] {
            int newPage = t2->getVal();

            if (newPage >= 1 && newPage <= static_cast<int>(p.size())) {
                int newIndex = newPage - 1;

                if (ps->getToggle()) {
                    p[curr]->setVisible(false);

                    curr = newIndex;
                    p[curr]->setVisible(true);
                }

                else if (hs->getToggle()) {
                    h[currH]->setVisible(false);

                    currH = newIndex;
                    h[currH]->setVisible(true);
                }

                sync();
            }

            else {
                t2->setVal(curr + 1);
            }
        }
    );

    auto crt = prop->add<Button>(Rect::percent(20, 0, 10, 100), "Create");
    crt->setCallback(
        [&] {
            if (ps->getToggle()) {
                p.push_back(app.add<Plot>(Rect::percent(1, 6, 98, 88), "Plot " + std::to_string(p.size() + 1)));
                p[p.size() - 1]->setTab(2);
                p[p.size() - 1]->setVisible(false);

                p[curr]->setVisible(false);
                curr = p.size() - 1;
                p[curr]->setVisible(true);

                t2->setVal(curr + 1);
            }

            else if (hs->getToggle()) {
                h.push_back(app.add<Histogram>(Rect::percent(1, 6, 98, 88), "Histogram " + std::to_string(h.size() + 1)));
                h[h.size() - 1]->setTab(2);
                h[h.size() - 1]->setVisible(false);

                h[currH]->setVisible(false);
                currH = h.size() - 1;
                h[currH]->setVisible(true);

                t2->setVal(currH + 1);
            }
        }
    );

    auto rmv = prop->add<Button>(Rect::percent(30, 0, 10, 100), "Remove");

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
            File file = File::open();
            if (!file.getName().empty()) {
                fs->add(file);
            }
        }
    );

    auto b7 = c1->add<Button>(Rect::percent(0, 92, 100, 4), "Link");
    b7->setCallback(
        [&] {
            if (!fs->getSelect()) {
                return;
            }

            Data& data = fs->getData();

            if (data.cname == "TGraph") {
                p[curr]->setData(data.x, data.y);
                p[curr]->setLable(data.name);
                p[curr]->setAxisX(data.axisX);
                p[curr]->setAxisY(data.axisY);
                p[curr]->reset();
            }

            else if (data.cname == "TH1F") {
                h[currH]->setData(data.x, data.y);
                h[currH]->setBins(data.bins);
                h[currH]->setLable(data.name);
                h[currH]->setAxisX(data.axisX);
                h[currH]->setAxisY(data.axisY);
                h[currH]->reset();
            }
        }
    );

    ps->setCallback(
        [&] {
            ps->setToggle(true);
            hs->setToggle(false);

            b1->setVisible(true);
            b2->setVisible(true);

            p[curr]->setVisible(true);
            h[currH]->setVisible(false);

            t2->setVal(curr + 1);

            sync();
        }
    );

    hs->setCallback(
        [&] {
            hs->setToggle(true);
            ps->setToggle(false);

            b1->setVisible(false);
            b2->setVisible(false);

            p[curr]->setVisible(false);
            h[currH]->setVisible(true);

            t2->setVal(currH + 1);

            sync();
        }
    );

    rmv->setCallback(
        [&] {
            if (ps->getToggle()) {
                if (p.size() <= 1) return;

                p[curr]->setVisible(false);

                p.erase(p.begin() + curr);

                if (curr >= static_cast<int>(p.size())) {
                    curr = static_cast<int>(p.size()) - 1;
                }

                p[curr]->setVisible(true);

                t2->setVal(curr + 1);

                sync();
            }

            else if (hs->getToggle()) {
                if (h.size() <= 1) return;

                h[currH]->setVisible(false);

                h.erase(h.begin() + currH);

                if (currH >= static_cast<int>(h.size())) {
                    currH = static_cast<int>(h.size()) - 1;
                }

                h[currH]->setVisible(true);

                t2->setVal(currH + 1);

                sync();
            }
        }
    );

    b3->setCallback(
        [&] {
            std::string path = File::save();
            if (path.empty()) return;

            std::cout << path << std::endl;

            if (ps->getToggle()) {
                p[curr]->save(path);
            }

            else if (hs->getToggle()) {
                h[currH]->save(path);
            }
        }
    );

    // View
    tl->setTab(3);
    tl->setCallback(
        [&] {
            if (ps->getToggle()) {
                p[curr]->setLable(tl->getText());
            }

            else if (hs->getToggle()) {
                h[currH]->setLable(tl->getText());
            }
        }
    );

    ax->setCallback(
        [&] {
            if (ps->getToggle()) {
                p[curr]->setAxisX(ax->getText());
            }

            else if (hs->getToggle()) {
                h[currH]->setAxisX(ax->getText());
            }
        }
    );

    ay->setCallback(
        [&] {
            if (ps->getToggle()) {
                p[curr]->setAxisY(ay->getText());
            }

            else if (hs->getToggle()) {
                h[currH]->setAxisY(ay->getText());
            }
        }
    );

    app.run();
}