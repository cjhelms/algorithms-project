/*
* Author: Mark Du, Chris Helms, Grant Gasser
* Assignment Title: Algorithms Project
* Assignment Description: Driver for visualization program for
*  -Brute force closest pair
*  -Brute Force convex hull
*  -Divide and Conquer closest pair
*  -Divide and Conquer convex hull
* Due Date: 5/5/2019
* Date Created: 3/5/2019
* Date Last Modified: 5/5/2019
*/

#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>
#include <sstream>
#include <cstdlib>
#include <time.h>
#include "line.h"
#include "bruteForceConvexHull.h"
#include "BruteForce.h"
#include "quickHull_convexHull.h"
#include "Divide_and_Conquer.h"
#include "UserInterface.h"

using namespace std;

/*
 * description: receives input from user
 * return: void
 * precondition: g has been initialized
 * postcondition: data has been stored in p
 */
void receiveInput( SDL_Plotter &g, vector<pair<int, int>> &p );

/*
 * description: cleans data provided from user
 * return: void
 * precondition: d has data
 * postcondition: duplicates have been removed from d
 */
void cleanData( vector<pair<int, int>> &d );

/*
 * description: runs algorithms based off user input
 * return: void
 * precondition: g has been initialized & p has data
 * postcondition: data is unchanged, results are displayed to screen
 */
void runAlgorithm( SDL_Plotter &g, vector<pair<int, int>> &p );

/**
 * description: Takes in input from user via mouse clicks
 *      or random generator
 * return: void
 * precondition: There is an SDL plotter and set of points that exist
 * postcondition: Points are stored into the data structure
 */
void receiveInput( SDL_Plotter &g, vector<pair<int, int>> &p );

/**
 * description: Clears the data in the set of points
 * return: void
 * precondition: There is a set of points d that exists
 * postcondition: The data is cleared
 */
void cleanData( vector<pair<int, int>> &d );

/**
 * description: Runs the algorithm visualizations
 * return: void
 * precondition: there is a set of data p, and SDL plotter that exists
 * postcondition: The selected algorithm is chosen
 */
void runAlgorithm( SDL_Plotter &g, vector<pair<int, int>> &p );

/**
 * Runs the main application
 * @param argc argument count
 * @param argv argument vector
 * @return int
 */
int main( int argc, char** argv ) {

    SDL_Plotter plotter( ROW_MAX, COL_MAX );
    vector<pair<int, int>> data;

    // display user interface
    userInterface( plotter );

    // while user has not requested to quit
    while (!plotter.getQuit()) {
        // receive data input from user
        Rectangle(Point(0,350), Point(GUI_X-2, 365), Color(255,255,255)).draw(plotter);
        writeWord( plotter, "RECEIVING INPUT", Color(255,0,0), 1, Point(0, 350));
        plotter.update();
//        cout << "receiving input...\n";
        receiveInput( plotter, data );

        // remove duplicates from data
//        cout << "cleaning data...\n";
        cleanData( data );

        // perform requested algorithm on data
        Rectangle(Point(0,350), Point(GUI_X-2, 365), Color(255,255,255)).draw(plotter);
        writeWord( plotter, "RUNNING ALGORITHMS", Color(255,0,0), 1, Point(0, 350));
        plotter.update();
//        cout << "running algorithms...\n";
        runAlgorithm( plotter, data );

        // reset display for next set of points
//        cout << "resetting...\n";
        clearScreen( plotter );

        // reset data for next set of points
        data = vector<pair<int, int>>();
    }

    return 0;
}

/*
 * description: receives input from user
 * return: void
 * precondition: g has been initialized
 * postcondition: data has been stored in p
 */
void receiveInput( SDL_Plotter &g, vector<pair<int, int>> &d ) {
    bool isDone = false;
    int mX = 0, mY = 0;

    // while user is inputting data
    while ( !g.getQuit() && !isDone ) {
        // if user hits enter
        if ( g.kbhit() ) {
            if ( g.getKey() == SDL_SCANCODE_RETURN ) {
                // exit receiveInput
                isDone = true;
            }
            else if ( g.getKey() == 'R' ) {
                // create RAN_PTS random points
                srand( ( unsigned ) time( NULL ) );
                for ( int i = 0; i < RAN_PTS; i++ ) {
                    d.push_back( make_pair( rand() % ( COL_MAX - GUI_X) + GUI_X,
                                            rand() % ROW_MAX ) );
                    point( d.back().first, d.back().second ).drawBig( g );
                    g.update();
                }
            }
        }

        // if user releases mouse button at a point on screen
        if ( g.getMouseUp( mX, mY ) && mX > GUI_X ) {
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

/*
 * description: cleans data provided from user
 * return: void
 * precondition: d has data
 * postcondition: duplicates have been removed from d
 */
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

/*
 * description: runs algorithms based off user input
 * return: void
 * precondition: g has been initialized & p has data
 * postcondition: data is unchanged, results are displayed to screen
 */
void runAlgorithm( SDL_Plotter &g, vector<pair<int, int>> &p ) {
    bool isDone = false;
    vector<point> points;

    while ( !g.getQuit() && !isDone ) {
        if ( g.kbhit() ) {
            // clear the screen & redraw the points
            clearScreen( g );
            for ( vector<pair<int, int>>::iterator i = p.begin();
                  i != p.end(); i++ ) {
                point( i->first, i->second ).drawBig( g );
            }
            g.update();

            switch ( g.getKey() ) {
                // brute-force closest-pair
                case '1':
//                    cout << "brute-force closest-pair\n";
                    bruteForce( g, p );
                    break;

                // divide-&-conquer closest-pair
                case '2':
//                    cout << "divide-&-conquer closest-pair\n";
                    divideAndConquerClosest(g, p);
                    break;

                // brute-force convex hull
                case '3':
//                    cout << "brute-force convex hull\n";
                    bruteForceConvexHull(g, p);
                    break;

                // divide-&-conquer convex hull
                case '4':
//                    cout << "divide-&-conquer convex hull\n";
                    divAndConqConvexHull( p, p.size(), g );
                    break;

                // user requested to exit runAlgorithm
                case SDL_SCANCODE_RETURN:
                    isDone = true;
                    break;
            }
        }
    }
}