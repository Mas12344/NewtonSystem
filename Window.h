#pragma once
#ifndef WINDOW_H

#include <gtkmm.h>

#include "Interval.h"
#include "NewtonSystem.h"

using namespace interval_arithmetic;

typedef long double (*fun_ld)(int, int, long double*);
typedef void (*dfun_ld)(int, int, long double*, long double*);
typedef void (*fun_iv)(int, int, long double*, long double*);
typedef void (*dfun_iv)(int, int, long double*, long double*);


enum DataType {
    FLOAT,
    INTERVAL,
    POINT_INTERVAL
};

class MyWindow : public Gtk::Window{
public:
    MyWindow();
    virtual ~MyWindow();

protected:
    void on_button_quit();
    void on_notebook_switch_page(Gtk::Widget* page, guint page_num);

    Gtk::Box m_VBox, m_RBBox;
    Gtk::Notebook m_Notebook;

    void on_dtype_rbutton_change();
    Gtk::RadioButton m_Float_RadioButton, m_Interval_RadioButton, m_PointInterval_RadioButton;

    Gtk::ButtonBox m_ButtonBox;
    Gtk::Button m_Button_Quit;

    Gtk::Label m_Label_FileChooser;

    //file chooser
    void on_button_file_clicked();

    Gtk::ButtonBox m_FileButtonBox;
    Gtk::Button m_Button_File;

    void on_entry_hmf_changed();
    Gtk::Label m_Label_How_Many_Functions;
    Gtk::Entry m_Entry_How_Many_Functions;

    //page 3 dane
    Gtk::Box m_DataBox;
    Gtk::Label m_Label_CurrentData;
    Gtk::Entry m_Entry_Data;
    Gtk::Button m_Button_NextData;
    void on_button_nextdata_clicked();

    //page4
    Gtk::Box m_Box_page4;
    Gtk::Label m_Label_mit, m_Label_eps;
    Gtk::Entry m_Entry_mit, m_Entry_eps;

    //page 5
    Gtk::Box m_WynikBox;
    Gtk::Button m_Button_Oblicz;
    Gtk::Label m_Label_WynikNapis, m_Label_Wyniki;
    void on_button_oblicz_clicked();

private:
    unsigned int NOFunctions=3;
    int dType = DataType::FLOAT;
    unsigned int dCounter = 0;
    uint16_t iterations = 0;

    fun_ld float_functions;
    dfun_ld float_derivatives;
    fun_iv interval_functions;
    dfun_iv interval_derivatives;

    long double wrraped_float_functions(uint16_t i, uint16_t n, std::vector<long double>* x);
    void wrraped_float_derivatives(uint16_t i, uint16_t n, std::vector<long double>* x, std::vector<long double>* out);
    Interval<long double> wrraped_interval_functions(int i, int n, std::vector<Interval<long double>>* x);
    void wrraped_interval_derivatives(int i, int n, std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* out);


    std::vector<long double> float_data;
    std::vector<Interval<long double>> interval_data;

};

#endif //WINDOW_H