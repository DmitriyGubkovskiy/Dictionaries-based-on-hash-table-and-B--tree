#include "Bplustree.h"



Inter_Node::~Inter_Node()
{
    for (int i = 0; i < M * 2 + 1; i++)
        Child[i] = NULL;
}

Node::~Node() {}
Leaf_Node::~Leaf_Node() {}
Bplus::~Bplus() {}
// Объект создания узла
Node::Node()
{
    isLeaf = true;
    count = 0;
    Parent = NULL;
}
//Объекты создания листового узла
Leaf_Node::Leaf_Node()
{
    isLeaf = true;
    Pre_Node = NULL;
    Next_Node = NULL;
}
//Промежуточные узлы создают объекты
Inter_Node::Inter_Node()
{
    isLeaf = false;
    for (int i = 0; i < M * 2 + 1; i++)
        Child[i] = NULL;
}
//Bplus Создание объектов
Bplus::Bplus()
{
    Root = NULL;
}

//Узел Поиск родственных узлов
Node* Node::GetBrother(int& flag)
{
    if (NULL == Parent)
        return NULL;

    Node* p = NULL;
    for (int i = 0; i <= Parent->count; i++)
    {
        if (Parent->Child[i] == this)
        {
            if (i == Parent->count)
            {
                p = Parent->Child[i - 1];//Флаг левых братьев=1
                flag = 1;
            }
            else
            {
                p = Parent->Child[i + 1];//Флаг правого брата=2
                flag = 2;
            }
        }
    }
    return p;
}

//Выход узла
void Node::Print(vector<pair<int, string>> key_words)
{
    
    for (int i = 0; i < this->count; i++)
    {
        string s;
        for (auto x: key_words)
        {
            if (x.first == key[i]) 
            {
                s = x.second; 
                break; 
            }
        }
        cout << s << " ";
        if (i >= count - 1)
            cout << " | ";
    }
}

//Разделение листовых узлов
int Leaf_Node::Split(Leaf_Node* p)
{
    int j = 0;
    for (int i = M; i < M * 2; i++, j++)//Скопируйте значение в новый узел
        p->key[j] = this->key[i];//это старый узел
    this->count = this->count - j;
    p->count = j;
    return p->key[0];
}

//Удаление листового узла
bool Leaf_Node::Delete(int value)
{
    bool found = false;
    int i = 0;
    for (; i < count; i++)
    {
        if (value == key[i])
        {
            found = true;
            break;
        }
    }
    if (false == found)
        return false;
    int j = i;
    for (; j < count - 1; j++)
        key[j] = key[j + 1];
    key[j] = 0;
    count--;
    return true;
}

//Вставка листовых узлов
bool Leaf_Node::Insert(int value)
{
    int i = 0;
    for (; (value > key[i]) && (i < count); i++)//по порядку
    {
    }
    for (int j = count; j > i; j--)//Переместите, чтобы найти место, куда следует вставить ключевое слово
        key[j] = key[j - 1];
    key[i] = value;//Вставить ключевое слово
    count++;
    return true;
}

//поиск узла листа
Leaf_Node* Bplus::Find(int data)//данные - ключевое слово
{
    int i = 0;
    Node* p = Root; 
    Inter_Node* q;  
    while (NULL != p)
    {
        if (p->isLeaf) //
            break;
        for (i = 0; i < p->count; i++) //
        {
            if (data < p->key[i])
                break;
        }
        q = (Inter_Node*)p;
        p = q->Child[i];
    }

    return (Leaf_Node*)p;//Возвращает корень, а если последователи пусты, то корнем будет узел, сгенерированный первой функцией вставки.
}

//Листовые узлы, объединение листовых узлов
bool Leaf_Node::Merge(Leaf_Node* p)
{
    if (this->count + p->count > M * 2)//Если они складываются вместе, это означает, что их не нужно объединять.
        return false;
    for (int i = 0; i < p->count; i++)//Иначе вставьте все ключевые слова oldnode в bro
        this->Insert(p->key[i]);
    return true;
}

//Промежуточное объединение узлов
bool Inter_Node::Merge(Inter_Node* p)
{
    key[count] = p->Child[0]->key[0];
    count++;
    Child[count] = p->Child[0];
    for (int i = 0; i < p->count; i++)
    {
        key[count] = p->key[i];
        count++;
        Child[count] = p->Child[i + 1];
    }
    return true;
}

//Вставка промежуточного узла
bool Inter_Node::Insert(int value, Node* New)
{
    int i = 0;
    for (; (i < count) && (value > key[i]); i++)//i указывает на место, куда должен быть вставлен ключ
    {
    }
    for (int j = count; j > i; j--)//сдвинуть что-л. назад
        key[j] = key[j - 1];
    for (int j = count + 1; j > i + 1; j--)//Значение ключа отца меняется, и ребенок перемещается;
        Child[j] = Child[j - 1];
    key[i] = value;//Ключевое слово передается отцовскому узлу
    Child[i + 1] = New;//Поместите новый узел на свое место.
    New->Parent = this;
    count++;
    return true;
}

//Разделение промежуточных узлов
int Inter_Node::Split(Inter_Node* p, int k)
{
    int i = 0, j = 0;
    if ((k > this->key[M - 1]) && (k < this->key[M]))//Раскол находится посередине.
    {
        for (i = M; i < M * 2; i++, j++)
            p->key[j] = this->key[i];//Скопируйте последнее значение в брат
        j = 1;
        for (i = M + 1; i <= M * 2; i++, j++)
        {
            this->Child[i]->Parent = p;//Ребенок двигается назад.
            p->Child[j] = this->Child[i];
        }
        this->count = M;//Вдвое меньше ключевых игроков
        p->count = M;
        return k;
    }
    int pos = k < this->key[M - 1] ? (M - 1) : M;//Посмотрите на сравнения k-size и middle-1 и найдите в передних или задних узлах
    k = this->key[pos];//Pos - точка разделения, позиционируется до или после точки разделения, и, наконец, должно быть промежуточным значением.
    j = 0;
    for (i = pos + 1; i < M * 2; i++, j++)//До узла и после узла, от позиции вставки, после вставки до нового узла.
        p->key[j] = this->key[i];
    j = 0;
    for (i = pos + 1; i <= M * 2; i++, j++)//Отдать ребенка брату
    {
        this->Child[i]->Parent = p;
        p->Child[j] = this->Child[i];
    }
    this->count = pos;
    p->count = M * 2 - pos - 1;
    return k;
}

//Удаление промежуточного узла
bool Inter_Node::Delete(int k)
{
    int i = 0;
    for (; (k >= key[i]) && (i < count); i++)
    {
    }
    for (int j = i - 1; j < count - 1; j++)
        key[j] = key[j + 1];
    k = i;
    for (; k < count; k++)
    {
        Child[k] = Child[k + 1];
    }
    Child[k] = NULL;
    count--;
    return true;
}

//промежуточный узел
bool Inter_Node::Slib(Inter_Node* p)
{
    int i, j;
    if (p->key[0] < this->key[0])
    {
        for (i = count; i > 0; i--)
            key[i] = key[i - 1];
        for (j = count + 1; j > 0; j--)
            Child[j] = Child[j - 1];
        key[0] = Child[0]->key[0];
        Child[0] = p->Child[p->count];
    }
    else
    {
        key[count] = p->Child[0]->key[0];
        Child[count + 1] = p->Child[0];
        for (i = 1; i < p->count - 1; i++)
            p->key[i - 1] = p->key[i];
        for (j = 0; j < p->count - 1; j++)
            p->Child[j] = p->Child[j + 1];
    }
    this->count++;
    p->count--;
    return true;
}

//Добавление узлов в дерево B+
bool Bplus::Add_Node(Inter_Node* p, int k, Node* New_Node)
{
    if (NULL == p || p->isLeaf)
        return false;
    if (p->count < M * 2)//Недовольство отца
        return p->Insert(k, New_Node);
    Inter_Node* Brother = new Inter_Node;
    //Листовой узел заполнен, родительский узел также заполнен Раздельный случай
    int NewKey = p->Split(Brother, k);//NewKey - это значение, которое будет извлечено и вставлено в корневой узел.

    //Определите ключевое слово, которое необходимо вставить, в каком месте узла разбиения оно должно быть вставлено
    if (p->count < Brother->count)
    {
        p->Insert(k, New_Node);
    }
    else if (p->count > Brother->count)
    {
        Brother->Insert(k, New_Node);
    }
    else
    {
        Brother->Child[0] = New_Node;
        New_Node->Parent = Brother;
    }
    Inter_Node* parent = (Inter_Node*)(p->Parent);
    if (NULL == parent)
    {
        parent = new Inter_Node();
        parent->Child[0] = p;
        parent->key[0] = NewKey;//newkey - это разделенный пассбэк, промежуточное значение для вставки
        parent->Child[1] = Brother;
        p->Parent = parent;
        Brother->Parent = parent;
        parent->count = 1;
        Root = parent;
        return true;
    }
    return Add_Node(parent, NewKey, Brother);
}

//Данные поиска дерева B+
bool Bplus::Search(int data)
{
    int i = 0;

    Node* p = Root;
    if (NULL == p)
        return false;
    Inter_Node* q;
    while (NULL != p)
    {
        if (p->isLeaf)
            break;
        for (i = 0; (i < p->count) && (data >= p->key[i]); i++)
        {
        }
        int k = i > 0 ? i - 1 : i;

        q = (Inter_Node*)p;
        p = q->Child[i];
    }
    if (NULL == p)
        return false;

    bool found = false;
    for (i = 0; i < p->count; i++)
    {
        if (data == p->key[i])
            found = true;
        
    }
    
    return found;
}

//Вставка дерева B+
bool Bplus::Insert(int data) //data - это ключевое слово для вставки
{

    string a;
    if (true == Search(data))//Найдите значение для вставки
        return false;

    Leaf_Node* Old_Node = Find(data);//Найдите листовой узел, который нужно вставить, определенный как oldnode

    if (NULL == Old_Node)
    {
        Old_Node = new Leaf_Node;//Дерево пустое
        Root = Old_Node;
    }

    if (Old_Node->count < M * 2) {//Есть место для вставки, прямой вставки и возврата
        return Old_Node->Insert(data);

    }

    Leaf_Node* New_Node = new Leaf_Node;//надвигающийся раскол

    int k = Old_Node->Split(New_Node);//k - первый ключевой ребенок нового узла

    Leaf_Node* OldNext = Old_Node->Next_Node;
    Old_Node->Next_Node = New_Node;//Соседние узлы листьев соединены
    New_Node->Next_Node = OldNext;
    New_Node->Pre_Node = Old_Node;

    if (NULL != OldNext)
        OldNext->Pre_Node = New_Node;

    if (data < k)//Меньше, чем newnode key[0], вставьте спереди, иначе вставьте сзади.
    {
        Old_Node->Insert(data);
    }
    else
    {
        New_Node->Insert(data);
    }
    Inter_Node* parent = (Inter_Node*)(Old_Node->Parent);

    if (NULL == parent)//Инициализируйте родительский узел, если родительский узел отсутствует, создайте новый.
    {
        Inter_Node* New_Root = new Inter_Node;
        New_Root->Child[0] = Old_Node;
        New_Root->key[0] = k;
        New_Root->Child[1] = New_Node;
        Old_Node->Parent = New_Root;
        New_Node->Parent = New_Root;
        New_Root->count = 1;
        Root = New_Root;
        return true;
    }

    return Add_Node(parent, k, New_Node);//Интерполяция в отца или расщепление отца для создания новых узлов
}

//Удаление деревьев B+
bool Bplus::Delete(int data)
{
    Leaf_Node* Old_Node = Find(data); //Найти данные
    if (NULL == Old_Node)//Дерево пустое
        return false;
    if (false == Old_Node->Delete(data)) //удаление
        return false;
    Inter_Node* parent = (Inter_Node*)(Old_Node->Parent);
    if (NULL == parent)
    {
        if (0 == Old_Node->count)//Удалите все дерево. Нет отца - нет ключа.
        {
            delete Old_Node;
            Root = NULL;
        }
        return true;
    }
    if (Old_Node->count >= M)
    {
        for (int i = 0; (i < parent->count) && (data >= parent->key[i]); i++)
        {
            if (parent->key[i] == data)//Если удаляемый ключ больше значения индекса отца, он будет удален напрямую, если равен, то отцу будет присвоен новый индекс.
                parent->key[i] = Old_Node->key[0];
        }
        return true;
    }
    // Не соответствует спецификации, объединить или заимствовать значения
    int flag = 1;
    Leaf_Node* Brother = (Leaf_Node*)(Old_Node->GetBrother(flag));
    int NewData = 0;
    if (Brother->count > M)//заёмная стоимость
    {
        if (1 == flag)//левая сторона (семейства)
        {
            NewData = Brother->key[Brother->count - 1];//Данные для заимствования
        }
        else//лит. правый брат
        {
            NewData = Brother->key[0];
        }
        Old_Node->Insert(NewData);
        Brother->Delete(NewData);
        //Замените значение ключа в родительском каталоге
        if (1 == flag)
        {
            for (int i = 0; i <= parent->count; i++)//Заимствование у левого брата
            {
                if (parent->Child[i] == Old_Node && i > 0)
                    parent->key[i - 1] = Old_Node->key[0];
            }
        }
        else
        {
            for (int i = 0; i <= parent->count; i++)//Заимствование у правого брата
            {
                if (parent->Child[i] == Old_Node && i > 0)
                    parent->key[i - 1] = Old_Node->key[0];
                if (parent->Child[i] == Brother && i > 1)
                    parent->key[i - 1] = Brother->key[0];
            }
        }
        return true;
    }
    int NewKey = 0;
    if (1 == flag)//Невозможность заимствования стоимости, консолидация
    {
        Brother->Merge(Old_Node);
        NewKey = Old_Node->key[0];//Отметить ключ в отце для удаления
        Leaf_Node* OldNext = Old_Node->Next_Node;//Заходи сзади, брат.
        Brother->Next_Node = OldNext;
        if (NULL != OldNext)
            OldNext->Pre_Node = Brother;
        delete Old_Node;
    }
    else
    {
        Old_Node->Merge(Brother);
        NewKey = Brother->key[0];
        Leaf_Node* OldNext = Brother->Next_Node;
        Old_Node->Next_Node = OldNext;
        if (NULL != OldNext)
            OldNext->Pre_Node = Old_Node;
        delete Brother;
    }
    return Remove_Node(parent, NewKey);//Удалить родителя или удалить ключевое слово в родителе;
}

//Bplus Remove Node
bool Bplus::Remove_Node(Inter_Node* p, int k)
{
    if (false == p->Delete(k))
    {
        return false;
    }
    Inter_Node* parent = (Inter_Node*)(p->Parent);
    if (NULL == parent)
    {
        if (0 == p->count)
        {
            Root = p->Child[0];
            delete p;
        }
        return true;
    }
    if (p->count >= M)//Отец не объединен
    {
        //Удалить ключевое слово в родительском тексте
        for (int i = 0; (i < parent->count) && (k >= parent->key[i]); i++)
        {
            if (parent->key[i] == k)//Проверьте, есть ли ключевые слова, которые нужно удалить в родительском падеже, и обновите индекс, если они есть.
                parent->key[i] = p->key[0];
        }
        return true;
    }
    //Слияние отцов
    int flag = 1;
    Inter_Node* Brother = (Inter_Node*)(p->GetBrother(flag));
    if (Brother->count > M)//Стоимость кредита отца
    {
        p->Slib(Brother);
        if (1 == flag)
        {
            for (int i = 0; i <= parent->count; i++)
            {
                if (parent->Child[i] == p && i > 0)
                    parent->key[i - 1] = p->key[0];
            }
        }
        else
        {
            for (int i = 0; i <= parent->count; i++)
            {
                if (parent->Child[i] == p && i > 0)
                    parent->key[i - 1] = p->key[0];
                if (parent->Child[i] == Brother && i > 0)
                    parent->key[i - 1] = Brother->key[0];
            }
        }
        return true;
    }
    //сллияние
    int NewKey = 0;
    if (1 == flag)
    {
        Brother->Merge(p);
        NewKey = p->key[0];
        delete p;
    }
    else
    {
        p->Merge(Brother);
        NewKey = Brother->key[0];
        delete Brother;
    }
    return Remove_Node(parent, NewKey);
}

//Выходной сигнал Bplus
void Bplus::Print(vector<pair<int, string>> key_words)
{
    Node* p = Root;
    if (NULL == p) 
    {
        cout << "Словарь пуст." << '\n';
        return;
    }
    Inter_Node* a;
    int H = 0;
    queue<Node*> q;
    queue<int> h;
    q.push(p);
    h.push(1);
    while (!q.empty())
    {
        p = q.front();
        if (H != h.front())
        {
            cout << endl;
            cout << H << ": ";
            H = h.front();
        }
        q.pop();
        h.pop();
        p->Print(key_words);
        if (NULL != p && !p->isLeaf)
        {
            a = (Inter_Node*)p;
            for (int i = 0; i <= p->count; i++)
            {
                q.push(a->Child[i]);
                h.push(H + 1);
            }
        }
    }
}


void Search(Bplus* bplus, string s)
{
    unsigned int data = crc32(s, s.size());
    

    if (bplus->Search(data))
    {
        cout << "Элемент присутствует в дереве." << endl;
    }
    else
    {
        cout << "Элемент отсутствует в дереве." << endl;
    }

}


void Add(Bplus* bplus, string s, vector<pair<int, string>>& key_words)
{
    
   
    unsigned int data = crc32(s, s.size());
    //TODO переделать
    bool success = bplus->Insert(data);
    key_words.push_back(pair<int, string>(data, s));
    
    if (true != success)
    {
        cout << "Элемент уже находится в дереве." << endl;
    }

}


void Del(Bplus* bplus, string s)
{
    unsigned int data = crc32(s, s.size());
    bool success = bplus->Delete(data);
    if (true == success)
    {
        cout << "Элемент успешно удален. " << endl;
    }
    else
    {
        cout << "Данного элемента нет в дереве. " << endl;
    }
    cout << endl;
}


void Print_tree(Bplus* bplus, vector<pair<int, string>> key_words)
{
    bplus->Print(key_words);
    cout << endl;
}

void From_txt(Bplus* bplus, vector<pair<int, string>> &key_words)
{
    ifstream in;
    string line;
    
    in.open("dict.txt");
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
                        for (int j = 0; j < tmp.size(); j++)
                        {
                            tmp[j] = tolower(tmp[j]);
                        }
                        
                        Add(bplus, tmp, key_words);
                        tmp.clear();
                    }
                    else if (tmp.size() > 1)
                    {
                        for (int j = 0; j < tmp.size(); j++)
                        {
                            tmp[j] = tolower(tmp[j]);
                        }
                        
                        Add(bplus, tmp, key_words);
                        tmp.clear();
                    }

                }
                i++;
            }
        }
    }
    
    in.close();
    std::cout << "Данные были записаны в словарь. " << std::endl;
}


unsigned int crc32(string str, unsigned int len) {

    unsigned int crc = 0xFFFFFFFF;

    const char* buffer = str.c_str();

    while (len--) {
        crc ^= *buffer++; // 8 млдаших бит crc
        for (int bit = 0; bit < 8; bit++) 
        { // проверка младшего бита
            if (crc & 1) crc = (crc >> 1) ^ 0xEDB88320; // младший бит = 1
            // -> сдвиг вправо + ксор с полиномом
            else crc = (crc >> 1); // младший бит = 0
            // -> сдвиг вправо
        }
    }

    return crc ^ 0xFFFFFFFF;
}