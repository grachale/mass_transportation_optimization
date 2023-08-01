#ifndef __PROGTEST__
#include <cassert>
#include <cstdint>
#include <iostream>
#include <memory>
#include <limits>
#include <optional>
#include <algorithm>
#include <bitset>
#include <list>
#include <array>
#include <vector>
#include <deque>
#include <set>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include <stack>
#include <queue>
#include <random>

template < typename F, typename S >
struct std::hash<std::pair<F, S>> {
    std::size_t operator () (const std::pair<F, S> &p) const noexcept {
// something like boost::combine would be much better
        return std::hash<F>()(p.first) ^ (std::hash<S>()(p.second) << 1);
    }
};

// For exposition only. In the tests Place will not
// be a string but some other type. This type will always
// be comparable, hashable and it will have default and
// copy constructors.
using Place = std::string;
using Connection = std::pair<Place, Place>;

struct Map {
    std::vector<Place> places;
    std::vector<Connection> connections;
};

#endif

using namespace std;
struct TrafficNetworkTester {
    explicit TrafficNetworkTester(const Map & currMap )
    {
        unordered_map<Place, unordered_set<Place>>  reversedCurrentNetwork;
        reversedCurrentNetwork.reserve(currMap.places.size());
        for ( const auto & place : currMap . places )
            reversedCurrentNetwork .emplace( move(place), unordered_set<Place>{});

        for ( const auto & connection : currMap . connections )
            reversedCurrentNetwork[connection . second ] . insert (move(connection . first) );

        // DFS part
        stack<Place> ourStack;
        unordered_set<Place> visited;
        unsigned int all = reversedCurrentNetwork . size();
        unsigned int counter1 = 0;

        // tip is count visited places to reduce for loop
        for (const auto & pair : reversedCurrentNetwork )
        {
            if ( visited . find ( pair . first ) == visited . end() )
            {
                DFS( pair . first, visited, reversedCurrentNetwork, ourStack, counter1);
                if ( counter1 == all )
                    break;
            }
        }

        reverseGraph ( reversedCurrentNetwork, currentNetwork );
        for ( const auto & vertex : currentNetwork )
            comp . emplace (vertex . first, 0 );

        counter = 0;
        while ( !ourStack . empty () )
        {
            Place station = ourStack . top();
            ourStack . pop ();

            if ( comp . find (station) == comp . end() )
            {
                ++counter;
                DFSComp (station, currentNetwork, comp, counter);
                if ( comp . size() == all )
                    break;
            }
        }

    }


    void DFS (const Place & vertex, unordered_set<Place> & visited, const unordered_map<Place, unordered_set<Place> > & graph, stack<Place> & Z, unsigned int & counter) const
    {
        stack<Place> ourStack;
        ourStack . push(vertex);

        while ( !ourStack . empty () )
        {
            Place current = ourStack . top ();

            visited . insert ( current );

            bool indicator = false;

            for ( const auto & neighbour : graph . find (current) -> second )
            {

                if ( visited . find (neighbour  ) == visited . end() )
                {
                    ourStack . push ( neighbour );
                    indicator = true;
                    break;
                }
            }
            if ( !indicator )
            {
                Z . push ( current );
                ++counter;
                ourStack . pop ();
            }
        }
    }


    void DFSComp (const Place & vertex, const unordered_map<Place, unordered_set<Place> > & graph, unordered_map<Place, unsigned int> & comp,  unsigned int & counter) const
    {
        stack<Place> ourStack;
        ourStack . push ( vertex );

        while ( !ourStack . empty () )
        {
            Place current = ourStack . top ();
            ourStack . pop ();

            comp[current] = counter;

            for ( const auto & neighbour : graph . find (current) -> second )
            {
                if ( comp[neighbour] == 0 )
                    ourStack . push ( neighbour );
            }
        }
    }

    void reverseGraph(const unordered_map<Place, unordered_set<Place>>& graph, unordered_map<Place, unordered_set<Place>>& reversedGraph) const {
        reversedGraph.reserve(graph.size());
        for (auto it = graph.begin(); it != graph.end(); ++it) {
            reversedGraph.emplace(it->first, unordered_set<Place>{});
        }

        for (auto it = graph.begin(); it != graph.end(); ++it) {
            const auto& from = it->first;
            const auto& toSet = it->second;
            for (auto setIt = toSet.begin(); setIt != toSet.end(); ++setIt) {
                auto& to = *setIt;
                reversedGraph[to].insert(from);
            }
        }
    }

    // Count how many areas exist in the network
    // after adding conns.
    // Note that conns may introduce new places.
    unsigned count_areas(const std::vector<Connection>& conns) const
    {
        // to do
        return 1;
    }

private:
    unordered_map<Place, unordered_set<Place> > currentNetwork;
    unordered_map<Place, unsigned int> comp;
    unsigned int counter;
};



#ifndef __PROGTEST__

using Test = std::pair<Map, std::vector<std::pair<unsigned, std::vector<Connection>>>>;

Test TESTS[] = {
        {
                { { "Dejvicka", "Hradcanska", "Malostranska", "Staromestska", "Mustek", "Muzeum" }, { // Map
                                                                                                            { "Dejvicka", "Hradcanska" }, { "Hradcanska", "Malostranska" },
                                                                                                            { "Malostranska", "Staromestska" }, { "Staromestska", "Mustek" },
                                                                                                            { "Mustek", "Muzeum" }
                                                                                                    } }, { // Queries
                                                                                                                 { 4, { { "Mustek", "Malostranska" } } },
                                                                                                                 { 4, { { "Malostranska", "Letnany" }, { "Letnany", "Dejvicka" } } },
                                                                                                                 { 1, { { "Malostranska", "Letnany" }, { "Letnany", "Dejvicka" }, { "Muzeum", "Hradcanska" } } },
                                                                                                                 { 1, { { "Muzeum", "Dejvicka" } } },
                                                                                                                 { 6, { { "Dejvicka", "Muzeum" } } },
                                                                                                         }
        },
        {
                { {
                    "Na Sklonku", "Poliklinika Čumpelíkova", "Šumavská", "Nové Podolí", "Vozovna Střešovice (Muzeum MHD)",
                                                                                        "Florenc", "Franty Kocourka", "Cukrovar Čakovice", "Praha-Dejvice", "Pod Říhákem",
                          "Sukovská", "Novoborská", "U Průhonu", "Nádraží Modřany",
                  }, {
                                                                                                            { "Pod Říhákem", "Novoborská" }, { "Nové Podolí", "Franty Kocourka" },
                                                                                                            { "Cukrovar Čakovice", "Florenc" }, { "Vozovna Střešovice (Muzeum MHD)", "Cukrovar Čakovice" },
                                                                                                            { "U Průhonu", "Praha-Dejvice" }, { "Sukovská", "Nové Podolí" },
                                                                                                            { "Poliklinika Čumpelíkova", "Nové Podolí" }, { "Florenc", "Na Sklonku" },
                                                                                                            { "Praha-Dejvice", "Cukrovar Čakovice" }, { "Franty Kocourka", "Šumavská" },
                                                                                                            { "Florenc", "Vozovna Střešovice (Muzeum MHD)" }, { "Pod Říhákem", "Florenc" },
                                                                                                            { "Novoborská", "Šumavská" }, { "Sukovská", "Šumavská" },
                                                                                                            { "Nové Podolí", "Pod Říhákem" }, { "Vozovna Střešovice (Muzeum MHD)", "Na Sklonku" },
                                                                                                            { "Cukrovar Čakovice", "Nádraží Modřany" }, { "Vozovna Střešovice (Muzeum MHD)", "Šumavská" },
                                                                                                            { "Novoborská", "Cukrovar Čakovice" }, { "Šumavská", "Na Sklonku" },
                                                                                                            { "Poliklinika Čumpelíkova", "Vozovna Střešovice (Muzeum MHD)" }, { "Nové Podolí", "Na Sklonku" },
                                                                                                            { "Pod Říhákem", "U Průhonu" }, { "Šumavská", "Sukovská" },
                                                                                                            { "Šumavská", "Nádraží Modřany" }, { "Nové Podolí", "Šumavská" },
                                                                                                            { "Poliklinika Čumpelíkova", "Šumavská" }, { "Šumavská", "Florenc" },
                                                                                                            { "Franty Kocourka", "Nové Podolí" }, { "U Průhonu", "Vozovna Střešovice (Muzeum MHD)" },
                                                                                                            { "Praha-Dejvice", "Florenc" }, { "Nové Podolí", "Praha-Dejvice" },
                                                                                                            { "Pod Říhákem", "Florenc" },
                                                                                                    } }, {
                                                                                                                 { 4, {
                                                                                                                      } },
                                                                                                                 { 2, {
                                                                                                                        { "Nádraží Modřany", "Poliklinika Čumpelíkova" },
                                                                                                                      } },
                                                                                                                 { 2, {
                                                                                                                        { "Na Sklonku", "Poliklinika Čumpelíkova" },
                                                                                                                      } },
                                                                                                                 { 1, {
                                                                                                                        { "Nádraží Modřany", "Poliklinika Čumpelíkova" }, { "Na Sklonku", "Poliklinika Čumpelíkova" },
                                                                                                                      } },
                                                                                                                 { 4, {
                                                                                                                        { "Poliklinika Čumpelíkova", "Na Sklonku" },
                                                                                                                      } },
                                                                                                                 { 2, {
                                                                                                                              { "Cukrovar Čakovice", "Poliklinika Čumpelíkova" }, { "Poliklinika Čumpelíkova", "Na Sklonku" },
                                                                                                                              { "Nádraží Modřany", "Poliklinika Čumpelíkova" },
                                                                                                                      } },
                                                                                                         }
        },
        {
                { {
                    "U Vojenské nemocnice", "Kuchyňka", "V Korytech", "Kelerka", "Vozovna Strašnice",
                                                                                        "Geologická", "U Studánky", "U Jahodnice", "Hadovka", "Barrandovská",
                          "K Netlukám", "Obchodní centrum Sárská", "Praha-Smíchov", "Sušická", "Moráň",
                          "Praha-Bubny", "Rajská zahrada", "Strossmayerovo náměstí", "Průmstav",
                  }, {
                                                                                                            { "U Vojenské nemocnice", "Vozovna Strašnice" }, { "K Netlukám", "Obchodní centrum Sárská" },
                                                                                                            { "Praha-Smíchov", "U Jahodnice" }, { "Praha-Smíchov", "K Netlukám" },
                                                                                                            { "Vozovna Strašnice", "Kelerka" }, { "Obchodní centrum Sárská", "Geologická" },
                                                                                                            { "K Netlukám", "Praha-Smíchov" }, { "V Korytech", "Geologická" },
                                                                                                            { "V Korytech", "Vozovna Strašnice" }, { "Vozovna Strašnice", "V Korytech" },
                                                                                                            { "U Vojenské nemocnice", "Kuchyňka" }, { "Kelerka", "Geologická" },
                                                                                                            { "Praha-Bubny", "Strossmayerovo náměstí" }, { "Kuchyňka", "V Korytech" },
                                                                                                            { "Praha-Smíchov", "Praha-Bubny" }, { "Obchodní centrum Sárská", "Moráň" },
                                                                                                            { "Kelerka", "V Korytech" }, { "Kelerka", "V Korytech" },
                                                                                                            { "Hadovka", "Rajská zahrada" }, { "V Korytech", "Geologická" },
                                                                                                            { "Sušická", "Praha-Smíchov" }, { "Barrandovská", "K Netlukám" },
                                                                                                            { "V Korytech", "Kelerka" }, { "K Netlukám", "V Korytech" },
                                                                                                            { "U Studánky", "Kuchyňka" }, { "Hadovka", "Barrandovská" },
                                                                                                            { "Praha-Bubny", "U Studánky" }, { "Moráň", "K Netlukám" },
                                                                                                            { "Strossmayerovo náměstí", "Kelerka" }, { "Barrandovská", "U Jahodnice" },
                                                                                                            { "V Korytech", "Kuchyňka" }, { "Průmstav", "Praha-Smíchov" },
                                                                                                            { "Geologická", "V Korytech" }, { "Rajská zahrada", "Kuchyňka" },
                                                                                                            { "U Jahodnice", "Kuchyňka" }, { "Praha-Smíchov", "Sušická" },
                                                                                                            { "K Netlukám", "Obchodní centrum Sárská" }, { "Geologická", "Kelerka" },
                                                                                                            { "Obchodní centrum Sárská", "K Netlukám" }, { "Obchodní centrum Sárská", "K Netlukám" },
                                                                                                            { "Hadovka", "U Studánky" }, { "K Netlukám", "Sušická" },
                                                                                                            { "Moráň", "U Vojenské nemocnice" }, { "Obchodní centrum Sárská", "Praha-Smíchov" },
                                                                                                            { "V Korytech", "U Studánky" }, { "Kuchyňka", "Geologická" },
                                                                                                            { "K Netlukám", "Moráň" }, { "Sušická", "U Vojenské nemocnice" },
                                                                                                            { "Kuchyňka", "U Vojenské nemocnice" },
                                                                                                    } }, {
                                                                                                                 { 9, {
                                                                                                                      } },
                                                                                                                 { 5, {
                                                                                                                        { "Kuchyňka", "Kuchyňka" }, { "Strossmayerovo náměstí", "Průmstav" },
                                                                                                                              { "Průmstav", "V Korytech" }, { "K Netlukám", "Praha-Smíchov" },
                                                                                                                              { "Praha-Bubny", "Barrandovská" },
                                                                                                                      } },
                                                                                                                 { 9, {
                                                                                                                        { "Rajská zahrada", "Strossmayerovo náměstí" }, { "Sušická", "Obchodní centrum Sárská" },
                                                                                                                                                                                  { "Průmstav", "Strossmayerovo náměstí" }, { "Moráň", "Strossmayerovo náměstí" },
                                                                                                                      } },
                                                                                                                 { 5, {
                                                                                                                        { "Kelerka", "K Netlukám" }, { "U Studánky", "Sušická" },
                                                                                                                              { "U Studánky", "V Korytech" }, { "U Studánky", "Strossmayerovo náměstí" },
                                                                                                                              { "Kuchyňka", "V Korytech" }, { "Průmstav", "Rajská zahrada" },
                                                                                                                      } },
                                                                                                                 { 5, {
                                                                                                                        { "Vozovna Strašnice", "Obchodní centrum Sárská" }, { "Strossmayerovo náměstí", "Praha-Bubny" },
                                                                                                                              { "U Vojenské nemocnice", "V Korytech" }, { "U Jahodnice", "U Studánky" },
                                                                                                                              { "Rajská zahrada", "V Korytech" }, { "Obchodní centrum Sárská", "Sušická" },
                                                                                                                      } },
                                                                                                                 { 2, {
                                                                                                                              { "Barrandovská", "Praha-Smíchov" }, { "Geologická", "Hadovka" },
                                                                                                                              { "U Studánky", "Moráň" }, { "U Vojenské nemocnice", "Praha-Smíchov" },
                                                                                                                      } },
                                                                                                         }
        },
        {
                { {
                    "Na Lukách", "Plánická", "U Mezníku", "Bílá Hora", "Psohlavců",
                                                                                        "Koupaliště Čakovice", "Volha", "Dolnopočernický hřbitov", "Studentský dům", "U Rozcestí",
                          "Koleje Jižní Město", "Kusá", "Vozovna Pankrác", "Ke Koulce", "Opatov",
                          "Nádraží Žvahov - V Násypu", "Na Blanici", "Nádraží Libeň", "Ořechovka", "Ke Kateřinkám",
                          "Divadlo pod Palmovkou", "Přístav Radotín", "Žákovská", "Pankrác", "Pod Děvínem",
                  }, {
                                                                                                            { "Koleje Jižní Město", "U Rozcestí" }, { "Na Lukách", "Psohlavců" },
                                                                                                            { "U Rozcestí", "U Mezníku" }, { "Ke Koulce", "Kusá" },
                                                                                                            { "Nádraží Žvahov - V Násypu", "Nádraží Libeň" }, { "Koleje Jižní Město", "U Rozcestí" },
                                                                                                            { "Koupaliště Čakovice", "Psohlavců" }, { "Žákovská", "Pankrác" },
                                                                                                            { "Kusá", "U Rozcestí" }, { "Psohlavců", "Bílá Hora" },
                                                                                                            { "Volha", "Na Lukách" }, { "Na Lukách", "Plánická" },
                                                                                                            { "Vozovna Pankrác", "Opatov" }, { "Koupaliště Čakovice", "Bílá Hora" },
                                                                                                            { "Ořechovka", "Na Blanici" }, { "Psohlavců", "U Mezníku" },
                                                                                                            { "U Mezníku", "Bílá Hora" }, { "Divadlo pod Palmovkou", "U Rozcestí" },
                                                                                                            { "Vozovna Pankrác", "U Mezníku" }, { "Psohlavců", "Koupaliště Čakovice" },
                                                                                                            { "Na Blanici", "Nádraží Žvahov - V Násypu" }, { "Na Lukách", "Bílá Hora" },
                                                                                                            { "Kusá", "Koleje Jižní Město" }, { "Nádraží Libeň", "Ke Koulce" },
                                                                                                            { "Opatov", "Ke Koulce" }, { "Přístav Radotín", "Koupaliště Čakovice" },
                                                                                                            { "Kusá", "Koleje Jižní Město" }, { "Pankrác", "Žákovská" },
                                                                                                            { "Vozovna Pankrác", "Koleje Jižní Město" }, { "Plánická", "Na Lukách" },
                                                                                                            { "Bílá Hora", "Koupaliště Čakovice" }, { "Kusá", "Vozovna Pankrác" },
                                                                                                            { "Koupaliště Čakovice", "Bílá Hora" }, { "Psohlavců", "Koleje Jižní Město" },
                                                                                                            { "Bílá Hora", "U Mezníku" }, { "Psohlavců", "Bílá Hora" },
                                                                                                            { "Koleje Jižní Město", "Kusá" }, { "Přístav Radotín", "Opatov" },
                                                                                                            { "Plánická", "U Mezníku" }, { "Vozovna Pankrác", "Nádraží Žvahov - V Násypu" },
                                                                                                            { "Koupaliště Čakovice", "Bílá Hora" }, { "Nádraží Žvahov - V Násypu", "Opatov" },
                                                                                                            { "Vozovna Pankrác", "Opatov" }, { "Studentský dům", "Volha" },
                                                                                                            { "Žákovská", "Pankrác" }, { "U Mezníku", "Na Lukách" },
                                                                                                            { "Ke Kateřinkám", "Koleje Jižní Město" }, { "U Mezníku", "Plánická" },
                                                                                                            { "Opatov", "Na Blanici" }, { "U Mezníku", "Volha" },
                                                                                                            { "Kusá", "Psohlavců" }, { "Kusá", "Ke Koulce" },
                                                                                                            { "Dolnopočernický hřbitov", "Bílá Hora" },
                                                                                                    } }, {
                                                                                                                 { 9, {
                                                                                                                      } },
                                                                                                                 { 8, {
                                                                                                                        { "Pod Děvínem", "U Rozcestí" }, { "Nádraží Žvahov - V Násypu", "Pod Děvínem" },
                                                                                                                      } },
                                                                                                                 { 7, {
                                                                                                                        { "Pankrác", "Pod Děvínem" }, { "U Rozcestí", "Žákovská" },
                                                                                                                                                                                  { "Pod Děvínem", "Nádraží Žvahov - V Násypu" },
                                                                                                                      } },
                                                                                                         }
        },
};

template < typename C >
void test(C&& tests, bool exact) {
    int fail = 0, ok = 0;

    for (auto&& [ map, test_cases ] : tests) {
        TrafficNetworkTester T{map};
        for (auto&& [ ans, conns ] : test_cases) {


            if (exact)
                (ans == T.count_areas(conns) ? ok : fail)++;
            else
                ((ans == 1) == (T.count_areas(conns) == 1) ? ok : fail)++;

        }
    }

    if (fail)
        std::cout << fail << " of " << fail + ok << " tests failed!" << std::endl;
    else
        std::cout << "All " << ok << " tests succeded!" << std::endl;
}

int main()
{
    test(TESTS, true);
}

#endif


