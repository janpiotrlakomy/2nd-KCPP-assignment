/*
 * Program
 */


/* Pliki nagłówkowe */

#include "poset.h"
#include <iostream>
#include <unordered_map>
#include <list>
#include <unordered_set>
#include <cstdint>

/* Deklaracja makr */

#define POSET_IDS poset_map().at(id).second;
#define POSET poset_map().at(id).first;

/* Deklaracje użycia wybranych elementów z przestrzeni std */

using std::cerr;
using std::string;
using std::unordered_set;
using std::pair;
using std::unordered_map;
using std::to_string;
using std::list;

/* Zadeklarowanie stałej mówiącej o zdefiniowaniu makra NDEBUG */

#ifdef NDEBUG
  bool constexpr debug = false;
#else
  bool constexpr debug = true;
#endif


/* Aliasy używanych typów */

using poset_ids_t = unordered_map<string, uint64_t>;
/*
    Pierwszy element pary - na kogo wskazuje element.
    Drugi element pary - kto wskazuje na element.
*/
using element_t = pair<unordered_set<uint64_t>, unordered_set<uint64_t>>;
using poset_t = unordered_map<uint64_t, element_t>;
using poset_map_t = unordered_map<unsigned long, pair<poset_t, poset_ids_t>>;


/* Funkcje pomocnicze */
namespace {

/*
* Zmienna zawierająca mapę posetów.
* Obudowana funkcją aby uniknąć 'static order initialization fiasco'.
*/
poset_map_t & poset_map() {
    poset_map_t temp1;
    static poset_map_t temp2 = temp1;
    return temp2;
}

/*
* Zmienna zawierająca największe id posetu występujące w poset_map.
* temp = 0 oznacza, że żaden poset nie został jeszcze dodany.
* Obudowana funkcją aby uniknąć 'static order initialization fiasco'.
*/
unsigned long & largest_id() {
    static unsigned long temp = -1;
    return temp;
}

/*
* Zmienna zawierająca największe element_id występujące w module.
* Obudowana funkcją aby uniknąć 'static order initialization fiasco'.
*/
uint64_t & largest_element_id() {
    static uint64_t temp = 0;
    return temp;
}

/*
* Funkcja sprawdza czy w posecie 'id' element
* 'value1' poprzedza element 'value2'.
*/
bool relation_test
(unsigned long id, uint64_t el1_id, uint64_t el2_id) {
    /* Jeśli relacja wynika ze zwrotności. */
    if (el1_id==el2_id)
        return true;

    poset_t &poset = POSET
    if (poset.at(el1_id).first.find(el2_id)
    == poset.at(el1_id).first.end())
        return false;

    return true;
}

/*
* Następujące procedury wypisują odpowiedni komunikat
* diagnostyczny, mówiący o wykonaniu funkcji func_name.
* Wartość pos to true jeśli wykonanie było poprawne
* (pozytywne), a false jeśli było niepoprawne.
*/
void print(string func_name, unsigned long id, bool pos) {
    if constexpr (!debug)
        return;
    cerr << func_name << ": poset ";

    if (func_name == "poset_new") {
        cerr << id << " created\n";
    }
    else if (func_name == "poset_delete") {
        if (pos)
            cerr << id << " deleted\n";
        else
            cerr << id << " does not exist\n";
    }
    else {
        cerr << id << " cleared\n";
    }
}

void print(string func_name, unsigned long id, string v, bool pos) {
    if constexpr (!debug)
        return;
    cerr << func_name << ": poset ";

    if (func_name == "poset_insert") {
        cerr << id << ", element \"" << v;
        if (pos)
            cerr << "\" inserted\n";
        else
            cerr << "\" already exists\n";
    }
    else {
        cerr << id << ", element \"" << v;
        cerr << "\" removed\n";
    }
}

void print(string func_name, unsigned long id, size_t size) {
    if constexpr (!debug)
        return;

    cerr << func_name << ": poset ";
    cerr << id << " contains ";
    cerr << size << " element(s)\n";
}

void print(string func_name, unsigned long id, string v1, string v2, bool pos) {
    if constexpr (!debug)
        return;
    cerr << func_name << ": poset ";
    cerr << id << ", relation (\"" << v1 << "\", \"" << v2;
    
    if (func_name == "poset_add") {
        if (pos)
            cerr << "\") added\n";
        else
            cerr << "\") cannot be added\n";
    }
    else if (func_name == "poset_del") {
        if (pos)
            cerr << "\") deleted\n";
        else
            cerr << "\") cannot be deleted\n";
    }
    else if (func_name == "poset_test") {
        if (pos)
            cerr << "\") exists\n";
        else
            cerr << "\") does not exist\n";
    }
}


/*
*   Funkcja sprawdza czy w posecie 'id' znajdują się elementy 'values'.
*/
bool element_doesnt_exist
(string func_name, unsigned long id, list<char const*> values){
    for (auto v: values) {
        if (poset_map().at(id).second.find(v)
        == poset_map().at(id).second.end()) {
            if constexpr (debug) {
                cerr << func_name << ": poset " << id;
                cerr << ", element \"" << v << "\" does not exist\n";
            }

            return true;
        }
    }
    return false;
}

/*
*   Funkcja sprawdza czy poset 'id' istnieje.
*/
bool poset_doesnt_exist(string func_name, unsigned long id){
    if (poset_map().find(id) == poset_map().end()) {
        if constexpr (debug) {
            cerr << func_name << ": poset ";
            cerr << id << " does not exist\n";
        }

        return true;
    }

    return false;
}

/*
*   Funkcja wypisuje stosowny komunikat dla wywołania funkcji 'func_name'
*/
void function_called
(string func_name, unsigned long id, list<char const*> values){
    string s = func_name + "(" + to_string(id);
    size_t count = 0;
    string a = "";
    for (auto i : values)
    {
        count++;
        s += ", ";
        if(i == nullptr) {
            s += "NULL";
            a += func_name + ": invalid value";
            if(values.size() > 1)
                a += to_string(count);
            a += + " (NULL)\n";
        }
        else {
            s += "\"" + string(i) + "\"";
        }
    }

    if constexpr (debug) {
    cerr << s << ")\n";
    cerr << a;
    }
}
}

/* Funkcje udostepniane przez moduł */

namespace cxx {

/*
 * Tworzy nowy poset i zwraca jego identyfikator.
 */
unsigned long poset_new(void){
    if constexpr (debug)
        cerr << "poset_new()\n";

    poset_t new_poset;
    poset_ids_t new_poset_ids;
    poset_map().insert({++largest_id(), {new_poset, new_poset_ids}});
    print(__func__, largest_id(), true);

    return largest_id();
}

/*
 * Jeżeli istnieje poset o identyfikatorze id, usuwa go,
 * a w przeciwnym przypadku nic nie robi.
 */
void poset_delete(unsigned long id){
    function_called(__func__, id, {});

    if (poset_map().find(id) != poset_map().end()) {
        poset_map().erase(id);
        print(__func__, id, true);
    }
    else {
        print(__func__, id, false);
    }
}

/*
 *  Jeżeli istnieje poset o identyfikatorze id, to wynikiem jest liczba jego
 *  elementów, a w przeciwnym przypadku 0.
 */
size_t poset_size(unsigned long id){
    function_called(__func__, id, {});

    if(poset_doesnt_exist(__func__, id))
        return false;

    size_t size = poset_map().at(id).first.size();
    print(__func__, id, size);
    return size;
}

/*
 * Jeżeli istnieje poset o identyfikatorze id i element value nie należy do
 * tego zbioru, to dodaje element do zbioru, a w przeciwnym przypadku nic nie
 * robi. Nowy element nie jest w relacji z żadnym elementem. Wynikiem jest
 * true, gdy element został dodany, a false w przeciwnym przypadku.
 */
bool poset_insert(unsigned long id, char const *value){
    function_called(__func__, id,{value});

    if(poset_doesnt_exist(__func__, id))
        return false;

    if (value == nullptr)
        return false;
    poset_ids_t &poset_ids = POSET_IDS

    if (poset_ids.find(value) == poset_ids.end()) {
        poset_t &poset = POSET
        uint64_t el_id = ++largest_element_id();
        element_t new_el;
        poset.insert(make_pair(el_id, new_el));
        poset_ids.insert({value, el_id});
        print(__func__, id, value, true);
        return true;
    }
    print(__func__, id, value, false);
    return false;
}


/*
 * Jeżeli istnieje poset o identyfikatorze id i element value należy do tego
 * zbioru, to usuwa element ze zbioru oraz usuwa wszystkie relacje tego
 * elementu, a w przeciwnym przypadku nic nie robi. Wynikiem jest true, gdy
 * element został usunięty, a false w przeciwnym przypadku.
 */
bool poset_remove(unsigned long id, char const *value) {
    function_called(__func__, id,{value});

    if(poset_doesnt_exist(__func__, id))
        return false;
    if (value == nullptr)
        return false;
    if (element_doesnt_exist(__func__, id, {value}))
        return false;

    poset_ids_t &poset_ids = POSET_IDS
    poset_t &poset = POSET
    uint64_t el_id = poset_ids.at(value);

    /*
        Wszystkim elementom, na których wskazuje usuwany element
        usuwamy informację, że na nie wskazuje.
    */
    for (auto it : poset.at(el_id).first)
        poset.at(it).second.erase(el_id);

    /*
        Wszystkim elementom, które wskazywały na usuwany element,
        usuwamy tę informacje.
    */
    for (auto it : poset.at(el_id).second)
        poset.at(it).first.erase(el_id);

    /* Czyścimy ten element */
    poset.erase(el_id);
    poset_ids.erase(value);
    print(__func__, id, value, true);

    return true;
}


/*
 * Jeżeli istnieje poset o identyfikatorze id oraz elementy value1 i value2
 * należą do tego zbioru i nie są w relacji, to rozszerza relację w taki
 * sposób, aby element value1 poprzedzał element value2 (domyka relację
 * przechodnio), a w przeciwnym przypadku nic nie robi. Wynikiem jest true,
 * gdy relacja została rozszerzona, a false w przeciwnym przypadku.
 */
bool poset_add(unsigned long id, char const *value1, char const *value2) {
    function_called(__func__, id,{value1, value2});

    if(poset_doesnt_exist(__func__, id))
        return false;

    if (value1 == nullptr || value2 == nullptr)
        return false;

    if(element_doesnt_exist(__func__, id, {value1, value2}))
        return false;

    poset_ids_t &poset_ids = POSET_IDS
    uint64_t el1_id = poset_ids.at(value1);
    uint64_t el2_id = poset_ids.at(value2);

    if (relation_test(id, el1_id, el2_id)
    || relation_test(id, el2_id, el1_id)) {
        print(__func__, id, value1, value2, false);
        return false;
    }

    poset_t &poset = POSET

    /* el1 teraz wskazuje na każdy element, na który wskazuje el2 */
    poset.at(el1_id).first.insert(
        poset.at(el2_id).first.begin(), poset.at(el2_id).first.end());

    /* Dla każdego elementu, na który wskazuje el2,
       jest teraz wskazywany przez el1.
    */
    for (auto it : poset.at(el2_id).first)
        poset.at(it).second.insert(el1_id);

    /* el1 teraz wskazuje na el2. */
    poset.at(el1_id).first.insert(el2_id);

    /*
        Dla każdego elementu na który wskazuje el2,
        każdy element wskazujący na el1 teraz wskazuje na niego.
    */
    for (auto it1 : poset.at(el2_id).first) {
        for (auto it2 : poset.at(el1_id).second) {
            poset.at(it2).first.insert(it1);
            poset.at(it1).second.insert(it2);
        }
    }

    /* el2 jest teraz wskazywane przez każdy element wskazujący na el1. */
    poset.at(el2_id).second.insert(
        poset.at(el1_id).second.begin(), poset.at(el1_id).second.end());

    /* Dla każdego elementu wskazującego na el1, wskazuje on teraz na el2. */
    for (auto it : poset.at(el1_id).second)
        poset.at(it).first.insert(el2_id);

    /* el2 jest teraz wskazywany przez element1. */
    poset.at(el2_id).second.insert(el1_id);

    print(__func__, id, value1, value2, true);

    return true;
}


/*
 * Jeżeli istnieje poset o identyfikatorze id, elementy value1 i value2
 * należą do tego zbioru, element value1 poprzedza element value2 oraz
 * usunięcie relacji między elementami value1 i value2 nie zaburzy warunków
 * bycia częściowym porządkiem, to usuwa relację między tymi elementami,
 * a w przeciwnym przypadku nic nie robi. Wynikiem jest true, gdy relacja
 * została zmieniona, a false w przeciwnym przypadku.
 */
bool poset_del(unsigned long id, char const *value1, char const *value2) {
    function_called(__func__, id,{value1, value2});

    if(poset_doesnt_exist(__func__, id)) {
        return false;
    }
    if (value1 == nullptr || value2 == nullptr)
        return false;

    if (element_doesnt_exist(__func__, id, {value1, value2}))
        return false;

    poset_ids_t &poset_ids = POSET_IDS
    uint64_t el1_id = poset_ids.at(value1);
    uint64_t el2_id = poset_ids.at(value2);

    /* Jeśli element value1 nie poprzedza value2. */
    if (!relation_test(id, el1_id, el2_id)) {
        print(__func__, id, value1, value2, false);
        return false;
    }

    poset_t &poset = POSET

    /* Jeśli próbujemy usunąć relację wynikającą ze zwrotności */
    if (el1_id==el2_id) {
        print(__func__, id, value1, value2, false);
        return false;
    }

    /*
        Jeśli wśród elementów, na które wskazuje el1
        dowolny element wskazuje na el2, to relacja el1->el2 jest przechodnia
        przez ten element, więc nie można jej usunąć.
    */
    for (auto it : poset.at(el1_id).first)
        if (poset.at(it).first.find(el2_id) != poset.at(it).first.end()) {
            print(__func__, id, value1, value2, false);
            return false;
        }

    /*
        Jeśli wśród elementów, które wskazują na el2 dowolny element
        jest wskazywany przez el1, to relacja el1->el2 jest przechodnia
        przez ten element, więc nie można jej usunąć.
    */
    for (auto it : poset.at(el2_id).second)
        if (poset.at(it).second.find(el1_id) != poset.at(it).second.end()) {
            print(__func__, id, value1, value2, false);
            return false;
        }

    /* element1 teraz nie wskazuje na el2 */
    poset.at(el1_id).first.erase(el2_id);

    /* element2 nie jest teraz wskazywany przez el1 */
    poset.at(el2_id).second.erase(el1_id);
    print(__func__, id, value1, value2, true);

    return true;
}


/*
 * Jeżeli istnieje poset o identyfikatorze id, elementy value1 i value2
 * należą do tego zbioru oraz element value1 poprzedza element value2, to
 * wynikiem jest true, a w przeciwnym przypadku false.
*/
bool poset_test(unsigned long id, char const *value1, char const *value2){
    function_called(__func__, id,{value1, value2});

    if(value1 == nullptr || value2 == nullptr)
        return false;
    if(poset_doesnt_exist(__func__, id))
        return false;
    if (element_doesnt_exist(__func__, id, {value1, value2}))
        return false;

    poset_ids_t &poset_ids = POSET_IDS
    uint64_t el1_id = poset_ids.at(value1);
    uint64_t el2_id = poset_ids.at(value2);

    /* Jeśli value1 poprzedza value2. */
    if(relation_test(id, el1_id, el2_id)) {
        print(__func__, id, value1, value2, true);
        return true;
    }

    print(__func__, id, value1, value2, false);
    return false;
}


/*
 * Jeżeli istnieje poset o identyfikatorze id, usuwa wszystkie jego elementy
 * oraz relacje między nimi, a w przeciwnym przypadku nic nie robi.
 */
void poset_clear(unsigned long id){
    function_called(__func__, id, {});

    if (poset_doesnt_exist(__func__, id))
        return;

    poset_ids_t &poset_ids = POSET_IDS
    poset_t &poset = POSET
    poset_ids.clear();
    poset.clear();

    print(__func__, largest_id(), true);
}

}