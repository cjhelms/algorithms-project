// 4/10/2019

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>
#include <algorithm>
#include <bits/stdc++.h>
#include "line.h"
#include "bruteForceConvexHull.h"
#include "dcConvexHull.h"

using namespace std;

const int ROW_MAX = 500, COL_MAX = 1000;

void receiveInput( SDL_Plotter &g, vector<pair<int, int>> &p );
void cleanData( vector<pair<int, int>> &d );
void runAlgorithm( SDL_Plotter &g, vector<pair<int, int>> &p );
vector<point> makePointVector(vector<pair<int, int>> &data);

// Main function
int main( int argc, char** argv ) {

    SDL_Plotter plotter( ROW_MAX, COL_MAX );
    vector<pair<int, int>> data;

    // while user has not requested to quit
    while (!plotter.getQuit()) {
        // receive data input from user
        cout << "receiving input...\n";
        receiveInput( plotter, data );

        // remove duplicates from data
        cout << "cleaning data...\n";
        cleanData( data );

        // perform requested algorithm on data
        cout << "running algorithms...\n";
        runAlgorithm( plotter, data );

        // reset display for next set of points
        cout << "resetting...\n";
        plotter.clear();
        plotter.update();

        // reset data for next set of points
        data = vector<pair<int, int>>();
    }

    return 0;
}

// Takes input for points using the mouse
void receiveInput( SDL_Plotter &g, vector<pair<int, int>> &d ) {
    bool isDone = false;
    int mX = 0, mY = 0;

    // while user is inputting data
    while ( !g.getQuit() && !isDone ) {
        // if user hits enter
        if ( g.kbhit() && g.getKey() == SDL_SCANCODE_RETURN ) {
            // exit receiveInput
            isDone = true;
        }

        // if user releases mouse button at a point on screen
        if ( g.getMouseUp( mX, mY ) ) {
            // push point onto vector
            d.push_back( make_pair( mX, mY ) );

            // draw point to screen
            point( mX, mY ).drawBig( g );

            // display point to console (for testing)
            //p.display( cout );

            // update screen with new point
            g.update();
        }
    }
}

// Create data (vector for holding the points)
void cleanData( vector<pair<int, int>> &d ) {
    unordered_set<string> goodPts;
    vector<int> goodNdx;
    stringstream convert;
    string str = "";
    int ndx = d.size() - 1;

    for ( int i = 0; i < ( int ) d.size(); i++ ) {
        convert.clear();

        // convert point to single string value
        convert << d[ i ].first << d[ i ].second;
        convert >> str;

        // if point has not already been found in d
        if ( goodPts.find( str ) == goodPts.end() ) {
            // add to set & mark as not a duplicate
            goodNdx.push_back( i );
            goodPts.insert( str );
        }
    }

    // remove duplicates from d
    vector<pair<int, int>> noDup;
    for ( vector<int>::iterator i = goodNdx.begin();
          i != goodNdx.end(); i++ ) {
        // add values with no duplicates to temporary vector
        noDup.push_back( d[ *i ] );

        // print value (for testing)
        //cout << "(" << d[ *i ].first << ", " << d[ *i ].second << ")\n";
    }
    d = noDup;
}

// Run algorithm based on 1-4
void runAlgorithm( SDL_Plotter &g, vector<pair<int, int>> &p ) {
    bool isDone = false;
    vector<point> points;
    clock_t timeTaken;

    while ( !g.getQuit() && !isDone ) {
        if ( g.kbhit() ) {
            // clear the screen & redraw the points
            g.clear();
            for ( vector<pair<int, int>>::iterator i = p.begin();
                  i != p.end(); i++ ) {
                point( i->first, i->second ).drawBig( g );
            }
            g.update();

            switch ( g.getKey() ) {
                // brute-force closest-pair
                case '1':
                    cout << "brute-force closest-pair\n";
                    break;

                // divide-&-conquer closest-pair
                case '2':
                    cout << "divide-&-conquer closest-pair\n";
                    break;

                // brute-force convex hull (clocked time)
                case '3':
                    timeTaken = clock();
                    cout << "brute-force convex hull\n";
                    bruteForceConvexHull(g, p);
                    timeTaken = clock() - timeTaken;
                    cout << "Time taken for brute force convex hull: "
                            << (float)timeTaken/CLOCKS_PER_SEC << endl;
                    break;

                // divide-&-conquer convex hull (clocked time)
                case '4':
                    timeTaken = clock();
                    cout << "divide-&-conquer convex hull\n";
                    points = makePointVector(p);
                    divAndConqConvexHull(g, points, p.size());
                    timeTaken = clock() - timeTaken;
                    cout << "Time taken for divide and conquer convex hull: "
                         << (float)timeTaken/CLOCKS_PER_SEC << endl;
                    points.clear();
                    break;

                // user requested to exit runAlgorithm
                case SDL_SCANCODE_RETURN:
                    isDone = true;
                    break;
            }
        }
    }
}

// Helper function to generate vector<points>
vector<point> makePointVector(vector<pair<int, int>> &data){
    vector<point> pointVector;
    for(auto i : data){
        point pt(i.first, i.second);
        pointVector.emplace_back(pt);
    }

    return pointVector;
}