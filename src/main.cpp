#include "application.hpp"
#include "container.hpp"
#include "text.hpp"
#include "button.hpp"
#include "plot.hpp"
#include "browser.hpp"
#include "process.hpp"

int main() {
    Application app(Rect::percent(0, 0, 90, 80), "Root Batch Viewer");

    std::vector<std::shared_ptr<Plot>> p;
    for (int i = 0; i < 20; ++i) {
        p.push_back(app.add<Plot>(Rect::percent(1, 6, 98, 88), "Plot " + std::to_string(i + 1)));
        p[i]->setTab(2);
        p[i]->setVisible(false);
        // p[i]->setData(x, y);
    }
    int curr = 0;
    p[curr]->setVisible(true);

    std::vector<float> hx;

    std::vector<std::shared_ptr<Histogram>> h;
    for (int i = 0; i < 20; ++i) {
        h.push_back(app.add<Histogram>(Rect::percent(1, 6, 98, 88), "Histogram " + std::to_string(i + 1)));
        h[i]->setTab(2);
        h[i]->setVisible(false);
        // h[i]->setData(hx, x);
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

    auto view_p = app.add<Container>(Rect::percent(1, 1, 20, 50), "View Panel");
    view_p->setTab(3);
    view_p->setColor(0.9, 0.9, 0.9, 1);

    auto tl = view_p->add<InputText>(Rect::percent(1, 1, 98, 6), "Title");
    tl->setText(p[curr]->getLable());

    auto ax = view_p->add<InputText>(Rect::percent(1, 8, 98, 6), "Axis X");
    ax->setText(p[curr]->getAxisX());

    auto ay = view_p->add<InputText>(Rect::percent(1, 15, 98, 6), "Axis Y");
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

// ===================================================================================================
// Algorithms

    DQMAlgoProcessor dqmProcessor;

    auto dqm_panel = app.add<Container>(Rect::percent(1, 1, 20, 50), "DQM Process");
    dqm_panel->setTab(1);
    dqm_panel->setColor(0.9, 0.9, 0.9, 1);

    auto dqm_type_input = dqm_panel->add<InputText>(Rect::percent(1, 1, 98, 6), "Algorithm Type");
    dqm_type_input->setText("Histogram Max");

    auto dqm_min_input = dqm_panel->add<InputText>(Rect::percent(1, 8, 98, 6), "Filter Min");
    dqm_min_input->setText("-100.0");

    auto dqm_max_input = dqm_panel->add<InputText>(Rect::percent(1, 15, 98, 6), "Filter Max");
    dqm_max_input->setText("100.0");

    auto run_one_b = dqm_panel->add<Button>(Rect::percent(1, 22, 98, 6), "Run One");
    
    static int uniq = 1;
    run_one_b->setCallback(
        [&] {
            std::vector<float> currentBinsPos = h[currH]->getY();
            std::vector<float> currentHeights = h[currH]->getX();
            std::string currentHistName = h[currH]->getLable();

            if (currentHeights.empty() || currentBinsPos.empty()) {
                std::cout << "[DQM Error] Current histogram on the screen is empty!" << std::endl;
                return;
            }

            float resX, resY;
            double minV = std::stod(dqm_min_input->getText());
            double maxV = std::stod(dqm_max_input->getText());

            bool ret = dqmProcessor.processSingleHistogram(
                dqm_type_input->getText(),
                currentHistName, 
                currentBinsPos,
                currentHeights,
                minV,
                maxV,
                resX,
                resY
            );

            if (ret) {

                h[currH]->setVisible(false);
                p[curr]->setVisible(true);

                std::vector<float> plotX = { resX }; 
                std::vector<float> plotY = { resY }; 
                p[curr]->setData(plotX, plotY);
                
                p[curr]->setLable("Algorithm: " + dqm_type_input->getText()  + " | "
                                                + "Name: " + currentHistName + " | " + std::to_string(uniq)
                );
                p[curr]->setAxisX("Current Run");
                p[curr]->setAxisY(dqm_type_input->getText());
                p[curr]->reset();

                ps->setToggle(true);
                hs->setToggle(false);
                b1->setVisible(true);
                b2->setVisible(true);

                uniq++;
                
                sync();

                std::cout << "[DQM Success] Calculation is performed for the histogram: " << currentHistName << " | Result = " << resY << std::endl;
            }
        }
    );

    auto run_all_b = dqm_panel->add<Button>(Rect::percent(1, 29, 98, 6), "Run All");
    run_all_b->setCallback(
        [&] {
            std::vector<float> plotX;
            std::vector<float> plotY;

            std::string algoName = dqm_type_input->getText();
            double minV = std::stod(dqm_min_input->getText());
            double maxV = std::stod(dqm_max_input->getText());

            int stepCounter = 1;
            
            for (size_t i = 0; i < h.size(); ++i) {
                if (!h[i]) continue;

                std::vector<float> currentBinsPos = h[i]->getY();
                std::vector<float> currentHeights = h[i]->getX();
                std::string currentHistName = h[i]->getLable();

                if (currentHeights.empty() || currentBinsPos.empty()) {
                    continue;
                }

                float resX, resY;

                bool ret = dqmProcessor.processSingleHistogram(
                    algoName,
                    currentHistName, 
                    currentBinsPos,
                    currentHeights,
                    minV,
                    maxV,
                    resX,
                    resY
                );

                if (ret) {
                    plotX.push_back(static_cast<float>(stepCounter));
                    plotY.push_back(resY);
                    stepCounter++;
                }
            }

            if (!plotY.empty()) {
                if (currH >= 0 && currH < static_cast<int>(h.size()) && h[currH]) {
                    h[currH]->setVisible(false);
                }
                
                if (curr < 0 || curr >= static_cast<int>(p.size()) || !p[curr]) {
                    std::cout << "[DQM Error] Invalid index!" << std::endl;
                    return;
                }
                p[curr]->setVisible(true);

                p[curr]->setData(plotX, plotY);
                
                p[curr]->setLable("Batch " + algoName + " | Total Points: " + std::to_string(plotY.size()) + " | #" + std::to_string(uniq));
                p[curr]->setAxisX("Histogram Index");
                p[curr]->setAxisY(algoName);
                
                p[curr]->reset();

                ps->setToggle(true);
                hs->setToggle(false);
                b1->setVisible(true);
                b2->setVisible(true);

                uniq++;
                
                sync();
            }
        }
    );

// ===================================================================================================

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

            if (ps->getToggle()) {
                p[curr]->save(path);
            }

            else if (hs->getToggle()) {
                h[currH]->save(path);
            }
        }
    );

    // View
    auto apply_view = view_p->add<Button>(Rect::percent(1, 22, 98, 6), "Apply");
    apply_view->setCallback(
        [&] {
            if (ps->getToggle()) {
                p[curr]->setLable(tl->getText().empty() ? " " : tl->getText());
                p[curr]->setAxisX(ax->getText().empty() ? " " : ax->getText());
                p[curr]->setAxisY(ay->getText().empty() ? " " : ay->getText());
            }

            else if (hs->getToggle()) {
                h[currH]->setLable(tl->getText().empty() ? " " : tl->getText());
                h[currH]->setAxisX(ax->getText().empty() ? " " : ax->getText());
                h[currH]->setAxisY(ay->getText().empty() ? " " : ay->getText());
            }
        }
    );

    app.run();
}