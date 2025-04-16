#include "Hash_Table.h"

void Hash_Table::Clear()
{
	this->size = 0;
	this->data.clear();
}

void Hash_Table::Add(string str)
{
  
    for (int j = 0; j < str.size(); j++)
    {
        str[j] = tolower(str[j]);
    }

    unsigned int hash = crc32(str, str.size());
    
    if (Cheak_Elements(hash)) 
    {
        for (int i = 0; i < this->size; i++)
        {
            if (this->data[i].first == hash and 
                !Cheak_In_Mass(this->data[i].second, str))
            /*if (this->data[i].first == hash)*/
            {
                this->data[i].second.push_back(str);
                this->size = this->data.size();
                break;
            }
        }
    }
    else
    {
        pair<unsigned int, vector<string>> new_pair;
        new_pair.first = hash;
        new_pair.second.push_back(str);
        this->data.push_back(new_pair);
        this->size = this->data.size();
    }
    
}

void Hash_Table::Delete(string str)
{
    unsigned int hash = crc32(str, str.size());

    if (Cheak_Elements(hash))
    {
        for (int i = 0; i < this->data.size(); i++)
        {
            if (this->data[i].first == hash)
            {
                if (this->data[i].second.size() > 1)
                {
                    for (int j = 0; j < data[i].second.size(); j++)
                    {
                        if (data[i].second[j] == str)
                        {
                            this->data[i].second.erase(
                                this->data[i].second.begin() + j);
                            break;
                        }
                    }
                    this->size = this->data.size();
                    break;
                }
                else
                {
                    this->data[i].second.clear();
                    this->data.erase(this->data.begin() + i);
                    this->size = this->data.size();
                    break;
                }
                
            }
        }
        cout << "Элемента был удален." << '\n';
    }
    else
    {
        cout << "Данного элемента нет в таблице." << '\n';
    }
}

void Hash_Table::Print()
{
    if (this->size == 0)
    {
        cout << "Словарь пуст." << '\n';
    }
    for (int i = 0; i < this->size; i++)
    {
        cout << this->data[i].first << ": ";
        cout << this->data[i].second[0];
        for (int j = 1; j < this->data[i].second.size(); j++)
        {
            cout << ", " << data[i].second[j];
        }
        cout << '\n';
    }
    

}

void Hash_Table::Searc(string str)
{

    unsigned int hash = crc32(str, str.size());


    if (Cheak_Elements(hash))
    {
        for (int i = 0; i < this->data.size(); i++)
        {
            if (hash == data[i].first)
            {
                cout << "Данная строка присутствует в словаре." ;
               
            }
        }
    }
    else
    {
        cout << "Данного элемента нет в таблице." << '\n';
    }

}

int Hash_Table::Size()
{
    return this->size;
}

void Hash_Table::To_txt()
{
    ofstream out;         
    out.open("dict.txt");      
    if (out.is_open())
    {
        for (int i = 0; i < this->size; i++)
        {
            out << this->data[i].first;
            for (int j = 0; j < data[i].second.size(); j++)
            {
                out << "/" << data[i].second[j];
            }
            out << '\n';
        }   
    }
    out.close();
    std::cout << "Данные были записаны в файл. " << std::endl;
}

void Hash_Table::From_txt()
{
    ifstream in;
    string line;
    this->Clear();
    in.open("dict.txt");
    //in.open("Палата 6.txt");
    if (in.is_open())
    {
        while (getline(in, line))
        {
            string tmp = "";
            int i = 0;
            
            while (i <= line.size())
            {
                
                if ((line[i] != ' ' and line[i] != ',' and line[i] != '.'
                    and line[i] != '!' and line[i] != '?' and line[i] != ';' and
                    line[i] != ':' and line[i] != '\n' and line[i] != '\0' and
                    line[i] != '-' and line[i] != '\"'))
                {
                    tmp += line[i];
                }
                else
                {
                    if (tmp.size() == 1 and tmp[0] != '-' and tmp[0] != ' ')
                    {
                        
                        this->Add(tmp);
                        tmp.clear();
                    }
                    else if (tmp.size() > 1)
                    {
                        
                        this->Add(tmp);
                        tmp.clear();
                    }
                   
                }
                i++;
            }
        }
    }
    this->size = this->data.size();
    in.close();
    std::cout << "Данные были записаны в словарь. " << std::endl;
}


Hash_Table::Hash_Table()
{
	this->size = 0;
}

Hash_Table::~Hash_Table()
{
	this->data.clear();
}


unsigned int Hash_Table::crc32(string str, unsigned int len) {

    unsigned int crc = 0xFFFFFFFF;

    const char* buffer = str.c_str();

    while (len--) {
        crc ^= *buffer++; // 8 млдаших бит crc
        for (int bit = 0; bit < 8; bit++) { // проверка младшего бита
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320; // младший бит = 1
            // -> сдвиг вправо + ксор с полиномом
            else crc = (crc >> 1); // младший бит = 0
            // -> сдвиг вправо
        }
    }
    return crc ^ 0xFFFFFFFF;
}

bool Hash_Table::Cheak_Elements(unsigned int hash)
{
    for (auto x : this->data)
    {
        if (x.first == hash)
        {
            return true;
        }
    }


    return false;
}

bool Hash_Table::Cheak_In_Mass(vector<string> vector, string str)
{
    for (auto x: vector)
    {
        if (x == str)return true;
    }
    return false;
}
