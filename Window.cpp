#include "Window.h"
#include <iostream>
#include <string>
#include <sstream>
#include <regex>
#include <functional>

#include <dlfcn.h>

using namespace std::placeholders;

MyWindow::MyWindow():
    m_VBox(Gtk::ORIENTATION_VERTICAL),
    m_RBBox(Gtk::ORIENTATION_VERTICAL),
    m_Button_Quit("Quit"),
    m_Float_RadioButton("Float"),
    m_Interval_RadioButton("Interval"),
    m_PointInterval_RadioButton("Point Interval"),
    m_FileButtonBox(Gtk::ORIENTATION_VERTICAL),
    m_Button_File("wybierz plik"),
    m_Label_FileChooser("wybierz plik .so z funkcjami"),
    m_Label_How_Many_Functions("ile funkcji"),
    m_DataBox(Gtk::ORIENTATION_HORIZONTAL),
    m_Button_NextData("Dalej"),
    m_Button_Oblicz("Oblicz"),
    m_Label_WynikNapis("Wynik: "),
    m_Label_Wyniki(""),
    m_WynikBox(Gtk::ORIENTATION_VERTICAL),
    m_Box_page4(Gtk::ORIENTATION_VERTICAL),
    m_Label_mit("Maksymalna ilosc iteracji"),
    m_Label_eps("Epsilon")
{
    
    set_title("Ukad rownan nieliniowych metoda Newtona!");
    set_border_width(10);
    set_default_size(400, 400);

    add(m_VBox);

    m_Notebook.set_border_width(10);
    m_VBox.pack_start(m_Notebook);
    m_VBox.pack_start(m_ButtonBox, Gtk::PACK_SHRINK);

    m_Entry_How_Many_Functions.set_max_length(5);
    m_Entry_How_Many_Functions.set_text("3");
    m_Entry_How_Many_Functions.select_region(0, m_Entry_How_Many_Functions.get_text_length());
    m_Entry_How_Many_Functions.signal_changed().connect(sigc::mem_fun(*this,
        &MyWindow::on_entry_hmf_changed));

    m_FileButtonBox.pack_start(m_Label_FileChooser);
    m_FileButtonBox.pack_start(m_Button_File);
    m_FileButtonBox.pack_start(m_Label_How_Many_Functions);
    m_FileButtonBox.pack_start(m_Entry_How_Many_Functions);

    m_Button_File.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_button_file_clicked));

    m_ButtonBox.pack_start(m_Button_Quit, Gtk::PACK_SHRINK);
    m_Button_Quit.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_button_quit));

    auto group = m_Float_RadioButton.get_group();
    m_Interval_RadioButton.set_group(group);
    m_PointInterval_RadioButton.set_group(group);

    m_Float_RadioButton.set_active();

    m_Float_RadioButton.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_dtype_rbutton_change));
    m_Interval_RadioButton.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_dtype_rbutton_change));
    m_PointInterval_RadioButton.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_dtype_rbutton_change));

    m_RBBox.pack_start(m_Float_RadioButton);
    m_RBBox.pack_start(m_Interval_RadioButton);
    m_RBBox.pack_start(m_PointInterval_RadioButton);

    //page 3
    std::stringstream ss;
    ss << "x[" << dCounter << "] = ";
    auto label_cd = Glib::ustring(ss.str());
    m_Label_CurrentData = Gtk::Label(label_cd);
    m_DataBox.pack_start(m_Label_CurrentData);
    m_DataBox.pack_start(m_Entry_Data);
    m_Button_NextData.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_button_nextdata_clicked));
    m_DataBox.pack_start(m_Button_NextData);

    //page 4
    m_Entry_mit.set_text("10");
    m_Entry_eps.set_text("1e-16");
    m_Box_page4.pack_start(m_Label_mit);
    m_Box_page4.pack_start(m_Entry_mit);
    m_Box_page4.pack_start(m_Label_eps);
    m_Box_page4.pack_start(m_Entry_eps);

    //page 5
    m_Button_Oblicz.signal_clicked().connect(sigc::mem_fun(*this,
        &MyWindow::on_button_oblicz_clicked));
    m_WynikBox.pack_start(m_Button_Oblicz);
    m_WynikBox.pack_start(m_Label_WynikNapis);
    m_WynikBox.pack_start(m_Label_Wyniki);

    
    m_Notebook.append_page(m_FileButtonBox, "Funkcje");
    m_Notebook.append_page(m_RBBox, "Typ danych");
    m_Notebook.append_page(m_DataBox, "Dane");
    m_Notebook.append_page(m_Box_page4, "Dane2");
    m_Notebook.append_page(m_WynikBox, "Wyniki");

    m_Notebook.signal_switch_page().connect(sigc::mem_fun(*this,
        &MyWindow::on_notebook_switch_page));
    

    
    show_all_children();

    
    NOFunctions = strtoul(m_Entry_How_Many_Functions.get_text().c_str(), NULL, 10);
    float_data.resize(NOFunctions);
    interval_data.resize(NOFunctions);
}

MyWindow::~MyWindow(){

}

void MyWindow::on_button_quit(){
    hide();
}

void MyWindow::on_notebook_switch_page(Gtk::Widget* page, guint page_num){
    
}


void MyWindow::on_button_file_clicked(){
    Gtk::FileChooserDialog dialog("wybieraj plika", Gtk::FILE_CHOOSER_ACTION_OPEN);
    dialog.set_transient_for(*this);

    dialog.add_button("_cancel", Gtk::RESPONSE_CANCEL);
    dialog.add_button("otwieraj", Gtk::RESPONSE_OK);

    auto filter_so = Gtk::FileFilter::create();
    filter_so->set_name("so files");
    filter_so->add_mime_type("application/x-sharedlib");
    dialog.add_filter(filter_so);

    int result = dialog.run();

    switch(result){
    case(Gtk::RESPONSE_OK):{
        std::string filename = dialog.get_filename();
        void* libHandle = dlopen(filename.c_str(), RTLD_LAZY);

        float_functions      = (fun_ld)  dlsym(libHandle, "testFunctions_ld");
        if(float_functions == NULL){
            std::cerr << "Nie znalazlem funkcji testFunctions_ld ." << std::endl;
        }
        float_derivatives    = (dfun_ld) dlsym(libHandle, "dtestFunctions_ld");
        if(float_derivatives == NULL){
            std::cerr << "Nie znalazlem funkcji dtestFunctions_ld ." << std::endl;
        }
        interval_functions   = (fun_iv)  dlsym(libHandle, "testFunctions_iv");
        if(interval_functions == NULL){
            std::cerr << "Nie znalazlem funkcji testFunctions_iv ." << std::endl;
        }
        interval_derivatives = (dfun_iv) dlsym(libHandle, "dtestFunctions_iv");
        if(interval_derivatives == NULL){
            std::cerr << "Nie znalazlem funkcji dtestFunctions_iv ." << std::endl;
        }

        break;
    }
    case(Gtk::RESPONSE_CANCEL):
        std::cout << "cancel clicked.\n";
        break;
    default:
        std::cout << "co sie stao\n";
    }
}

void MyWindow::on_entry_hmf_changed(){
    NOFunctions = strtoul(m_Entry_How_Many_Functions.get_text().c_str(), NULL, 10);   
}

void MyWindow::on_dtype_rbutton_change(){
    bool b_float = m_Float_RadioButton.get_active();
    bool b_interval = m_Interval_RadioButton.get_active();
    bool b_point_interval = m_PointInterval_RadioButton.get_active();

    dCounter = 0;
    float_data.clear();
    float_data.resize(NOFunctions);
    interval_data.clear();
    interval_data.resize(NOFunctions);

    if(b_float){
        dType = DataType::FLOAT;
        auto label_cd = Glib::ustring("x[0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }

    if(b_interval){
        dType = DataType::INTERVAL;
        auto label_cd = Glib::ustring("x[0].a = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }

    if(b_point_interval){
        dType = DataType::POINT_INTERVAL;
        auto label_cd = Glib::ustring("x[0] = ");
        m_Label_CurrentData.set_text(label_cd);
        return;
    }
}

void MyWindow::on_button_nextdata_clicked(){
    setlocale(LC_ALL, "en-US");
    std::stringstream ss;
    std::size_t pos;

    // czy nie za duzo i bledy wpisania
    if (dCounter >= NOFunctions){
        m_Label_CurrentData.set_text("Koniec");
        return;
    }

    switch (dType)
    {
    case DataType::FLOAT:
        {

        auto s_val = m_Entry_Data.get_text();
        auto findAndReplaceAll = [](Glib::ustring& str, char* z, char* na){
            size_t pos = str.find(z);

            while(pos != std::string::npos){
                str.replace(pos, strlen(z), na);
                pos = str.find(z, pos+strlen(na));
            }
        };

        for(auto n: s_val){
            switch(n){
                case '.':
                findAndReplaceAll(s_val, ".", ",");
                case ',':
                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'e':
                case 'E':
                case 'd':
                case 'D':
                break;
                default:
                return;
            }
        }

        long double val = std::stold(s_val, &pos);        

        float_data[dCounter++] = val;
       
        ss << "x[" << dCounter << "] = ";
        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");
        
        }
        break;
    
    case DataType::POINT_INTERVAL:
        {

        auto s_val = m_Entry_Data.get_text();

        for(auto n: s_val){
            switch(n){
                case '.':
                case ',':
                case '-':
                case '0':
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                case 'e':
                case 'E':
                case 'd':
                case 'D':
                break;
                default:
                return;
            }
        }
        
        Interval<long double> tmp = IntRead<long double>(std::string(s_val));

        interval_data[dCounter++] = tmp;
        ss << "x[" << dCounter << "] = ";

        auto label_cd = Glib::ustring(ss.str());
        ss.clear();
        m_Label_CurrentData.set_text(label_cd);
        m_Entry_Data.set_text("");

        }
        break;

    case DataType::INTERVAL:
        {
        if(m_Label_CurrentData.get_text()[5] == 'b'){
            ss << "x[" << dCounter+1 << "].a = ";
            auto label_cd = Glib::ustring(ss.str());
            ss.clear();
            m_Label_CurrentData.set_text(label_cd);

            auto s_val = m_Entry_Data.get_text();
            for(auto n: s_val){
                switch(n){
                    case '.':
                    case ',':
                    case '-':
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case 'e':
                    case 'E':
                    case 'd':
                    case 'D':
                    break;
                    default:
                    return;
                }
            }
            
            interval_data[dCounter++].b = RightRead<long double>(std::string(s_val));
            m_Entry_Data.set_text("");

        }else{
            ss << "x[" << dCounter << "].b = ";
            auto label_cd = Glib::ustring(ss.str());
            ss.clear();
            m_Label_CurrentData.set_text(label_cd);

            auto s_val = m_Entry_Data.get_text();

            for(auto n: s_val){
                switch(n){
                    case '.':
                    case ',':
                    case '-':
                    case '0':
                    case '1':
                    case '2':
                    case '3':
                    case '4':
                    case '5':
                    case '6':
                    case '7':
                    case '8':
                    case '9':
                    case 'e':
                    case 'E':
                    case 'd':
                    case 'D':
                    break;
                    default:
                    return;
                }
            }
            
            Interval<long double> tmp;
            tmp.a = LeftRead<long double>(std::string(s_val));

            interval_data[dCounter] = tmp;
            m_Entry_Data.set_text("");

        }
        
        }
        break;
    default:
        break;
    }
}

long double MyWindow::wrraped_float_functions(uint16_t i, uint16_t n, std::vector<long double>* x){
    long double* raw_data = (long double*)malloc(x->size() * sizeof(long double));
    for(int i = 0; i < x->size(); i++){
        raw_data[i] = x->at(i);
    }
    long double ret = float_functions(i, n, raw_data);

    free(raw_data);
    return ret;
}

void MyWindow::wrraped_float_derivatives(uint16_t i, uint16_t n, std::vector<long double>* x, std::vector<long double>* out){
    long double* raw_data = (long double*)malloc(x->size() * sizeof(long double));
    long double* output = (long double*)malloc(out->size() * sizeof(long double));
    for(int i = 0; i < x->size(); i++){
        raw_data[i] = x->at(i);
    }
    float_derivatives(i, n, raw_data, output);

    for(int i = 0; i < out->size(); i++){
        (*out)[i] = output[i];
    }

    free(raw_data);
    free(output);
}

Interval<long double> MyWindow::wrraped_interval_functions(int i, int n, std::vector<Interval<long double>>* x){
    long double* y = (long double*)malloc(2 * n * sizeof(long double));
    for(int i = 0; i < n; i++){
        y[2*i + 0] = x->at(i).a;
        y[2*i + 1] = x->at(i).b;
    }

    long double* out = (long double*)malloc(2 * sizeof(long double));
    interval_functions(i, n, y, out);
    Interval<long double> ret = Interval<long double>(out[0], out[1]);
    free(y);
    free(out);
    return ret;

}

void MyWindow::wrraped_interval_derivatives(int i, int n, std::vector<Interval<long double>>* x, std::vector<Interval<long double>>* out){
    long double* y = (long double*)malloc(2 * n * sizeof(long double));
    std::vector<Interval<long double>>& outRef = *out;
    for(int i = 0; i < n; i++){
        y[2*i + 0] = x->at(i).a;
        y[2*i + 1] = x->at(i).b;
    }

    long double* out_ld = (long double*)malloc(2 * n * sizeof(long double));

    interval_derivatives(i, n, y, out_ld);

    for(int i = 0; i < n; i++){
        outRef[i] = Interval<long double>(out_ld[2*i + 0], out_ld[2*i + 1]);
    }
    free(y);
    free(out_ld);

}

void MyWindow::on_button_oblicz_clicked(){
    int max_iter = std::stoi(m_Entry_mit.get_text());
    stringstream ss;
    Gtk::MessageDialog flagdialog(*this, "Error");
    flagdialog.set_secondary_text("Macierz jest osobliwa");
    switch (dType)
    {
    case DataType::FLOAT:
        {
            auto f = std::bind(&MyWindow::wrraped_float_functions, this, _1, _2, _3);
            auto df = std::bind(&MyWindow::wrraped_float_derivatives, this, _1, _2, _3, _4);
            long double epsilon = std::stold(m_Entry_eps.get_text());
            int flag = ld::NewtonSystem(NOFunctions, &float_data, f, df, max_iter, epsilon, &iterations);
            for(int i = 0; i < NOFunctions; i++){
                ss << "x[" << i << "] = " << std::scientific << float_data[i] << "\n";
            }

            ss << "iteracje: " << iterations;
            if (flag == 0){
                m_Label_Wyniki.set_text(ss.str());
            }else{
                flagdialog.run();
            }
            ss.clear();
            
        }
        break;
    case DataType::INTERVAL:
    case DataType::POINT_INTERVAL:
        {
            auto f = std::bind(&MyWindow::wrraped_interval_functions, this, _1, _2, _3);
            auto df = std::bind(&MyWindow::wrraped_interval_derivatives, this, _1, _2, _3, _4);
            auto eps = IntRead<long double>(std::string(m_Entry_eps.get_text()));
            int flag = iv::NewtonSystem(NOFunctions, &interval_data, f, df, max_iter, eps, &iterations);
            
            for(int i = 0; i < NOFunctions; i++){
                std::string left, right;
                interval_data[i].IEndsToStrings(left, right);
                ss << "x[" << i << "] = " << left << " " << right << " szerokosc: " << IntWidth(interval_data[i]) << "\n";
            }

            ss << "iteracje: " << iterations;

            if (flag != 2){
                m_Label_Wyniki.set_text(ss.str());
            }else{
                flagdialog.run();
            }
            ss.clear();

        }
        break;
    default:
        std::cerr << "?\n";
        break;
    }
}