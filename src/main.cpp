#include "application.hpp"
#include "container.hpp"
#include "text.hpp"
#include "button.hpp"
#include "plot.hpp"

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

    auto prop = app.add<Container>(Rect::percent(1, 1, 98, 4), "Properties");
    prop->setTab(2);

    auto b1 = prop->add<ToggleButton>(Rect::percent(70, 0, 10, 100), "Log X");
    b1->setCallback([&p, &curr] { p[curr]->setLogX(!p[curr]->getLogX()); });

    auto b2 = prop->add<ToggleButton>(Rect::percent(80, 0, 10, 100), "Log Y");
    b2->setCallback([&p, &curr] { p[curr]->setLogY(!p[curr]->getLogY()); });

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
        [&p, &curr, &t2, &sync] {
            if (curr > 0) {
                p[curr]->setVisible(false);
                p[--curr]->setVisible(true);

                t2->setVal(curr + 1);

                sync();
            }
        }
    );

    auto b5 = nav->add<Button>(Rect::percent(55, 0, 45, 100), "Next");
    b5->setCallback(
        [&p, &curr, &t2, &sync] {
            if (curr < p.size() - 1) {
                p[curr]->setVisible(false);
                p[++curr]->setVisible(true);

                t2->setVal(curr + 1);

                sync();
            }
        }
    );

    app.run();
}