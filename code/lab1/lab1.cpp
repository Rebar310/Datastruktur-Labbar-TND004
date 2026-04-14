/*
 * lab1.cpp : stable partition
 * Iterative and divide-and-conquer
 */

#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <format>
#include <functional>
#include <cassert>


/****************************************
 * Declarations                          *
 *****************************************/

// generic class to write an item to a stream
template <typename T>
class Formatter {
public:
    Formatter(std::ostream& os, int width, int per_line)
        : os_{os}, per_line_{per_line}, width_{width} {
    }

    void operator()(const T& t) {
        os_ << std::format("{:{}}", t, width_);
        if (++outputted_ % per_line_ == 0)
            os_ << "\n";
    }

private:
    std::ostream& os_;    // output stream
    const int per_line_;  // number of columns per line
    const int width_;     // column width
    int outputted_{0};    // counter of number of items written to os_
};

/* ************************ */

namespace TND004 {
// Iterative algorithm
void stable_partition_iterative(std::vector<int>& V, std::function<bool(int)> p);

// Auxiliary function that performs the stable partition recursively
std::vector<int>::iterator stable_partition(std::vector<int>::iterator first,
                                            std::vector<int>::iterator last,
                                            std::function<bool(int)> p);

// Divide-and-conquer algorithm
void stable_partition(std::vector<int>& V, std::function<bool(int)> p) {
    TND004::stable_partition(std::begin(V), std::end(V), p);  // call auxiliary function
}
}  // namespace TND004

// Used for testing
void execute(std::vector<int>& V, const std::vector<int>& res);

bool even(int i);

/****************************************
 * Main:test code                        *
 *****************************************/

int main() {
    /*****************************************************
     * TEST PHASE 1                                       *
     ******************************************************/
    {
        std::cout << "TEST PHASE 1\n\n";

        std::vector<int> seq{1, 2};

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{2, 1});

        std::cout << "\nEmpty sequence: ";
        std::vector<int> empty;

        execute(empty, std::vector<int>{});
    }

    /*****************************************************
     * TEST PHASE 2                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 2\n\n";

        std::vector<int> seq{2};

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{2});
    }

    /*****************************************************
     * TEST PHASE 3                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 3\n\n";

        std::vector<int> seq{3};

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>{std::cout, " "});

        execute(seq, std::vector<int>{3});
    }

    /*****************************************************
     * TEST PHASE 4                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 4\n\n";

        std::vector<int> seq{3, 3};

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>(std::cout, " "));

        execute(seq, std::vector<int>{3, 3});
    }

    /*****************************************************
     * TEST PHASE 5                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 5\n\n";

        std::vector<int> seq{1, 2, 3, 4, 5, 6, 7, 8, 9};

        std::cout << "Sequence: ";
        std::copy(std::begin(seq), std::end(seq), std::ostream_iterator<int>(std::cout, " "));

        execute(seq, std::vector<int>{2, 4, 6, 8, 1, 3, 5, 7, 9});
    }

    /*****************************************************
     * TEST PHASE 6                                       *
     ******************************************************/
    {
        std::cout << "\n\nTEST PHASE 6: test with long sequence loaded from a file\n\n";

        std::ifstream file("C:/skola/TND004/Datastruktur-Labbar-TND004/code/lab1/test_data.txt"); // if mac then change this path

        if (!file) {
            std::cout << "Could not open test_data.txt!!\n";
            return 0;
        }

        // read the input sequence from file
        std::vector<int> seq{std::istream_iterator<int>{file}, std::istream_iterator<int>()};
        file.close();

        std::cout << "\nNumber of items in the sequence: " << std::ssize(seq) << '\n';

        /*std::cout << "Sequence:\n";
        std::for_each(std::begin(seq), std::end(seq), Formatter<int>(std::cout, 8, 5));*/

        // read the result sequence from file
        file.open("C:/skola/TND004/Datastruktur-Labbar-TND004/code/lab1/test_result.txt");  // if mac then change this path

        if (!file) {
            std::cout << "Could not open test_result.txt!!\n";
            return 0;
        }

        std::vector<int> res{std::istream_iterator<int>{file}, std::istream_iterator<int>()};

        std::cout << "\nNumber of items in the result sequence: " << std::ssize(res);

        // display expected result sequence
        // std::for_each(std::begin(res), std::end(res), Formatter<int>(std::cout, 8, 5));

        assert(std::ssize(seq) == std::ssize(res));

        execute(seq, res);
    }
}

/****************************************
 * Functions definitions                 *
 *****************************************/
//returnerae true om i är jämn, annars false
bool even(int i) {
    return i % 2 == 0;
}

// Used for testing
void execute(std::vector<int>& V, const std::vector<int>& res) {
    std::vector<int> copy_{V};

    std::cout << "\n\nIterative stable partition\n";
    TND004::stable_partition_iterative(V, even);
    assert(V == res);  // compare with the expected result

    
    // Uncomment for exercise 2
    std::cout << "Divide-and-conquer stable partition\n";
    TND004::stable_partition(copy_, even);
    assert(copy_ == res);  // compare with the expected result
    
}

// Iterative algorithm
void TND004::stable_partition_iterative(std::vector<int>& V, std::function<bool(int)> p) {
    // IMPLEMENT before Lab1 HA
	std::vector<int> stableSortedVector; //en tom vector som kommer att innehålla de sorterade värdena

	//Loopar igenom hela vektorn och lägger till de jämna värdena i den nya vektorn i deras ordning
    for (int i = 0; i < V.size(); i++) {
        if (p(V[i]) == true) {
			stableSortedVector.push_back(V[i]); //lägger till sist i den nya vektorn, ordning bevaras
        }
    }

	// De som inte var jämna (false) läggs till i den nya vektorn i deras ordning
    for (int i = 0; i < V.size(); i++) {
        if (p(V[i]) == false) {
            stableSortedVector.push_back(V[i]);
        }
    }

	// den gammal vektorn V ersätts med den nya vektorn som nu är stabilt sorterad
    V = stableSortedVector;

}




/*
 * Auxiliary function that performs the stable partition recursively
 * Divide-and-conquer algorithm: stable-partition the sub-sequence starting at first and ending
 * at last-1.
 * If there are items with property p then return an iterator to the end of the block
 * containing the items with property p. Otherwise, return first.
 */
std::vector<int>::iterator TND004::stable_partition(std::vector<int>::iterator first,
                                                    std::vector<int>::iterator last,
                                                    std::function<bool(int)> p) {
    // IMPLEMENT
	// Base case 1: empty seqence, finns inget returnera first
    if (first == last) {
        return first;
    }

	// Base case 2: one element, iteratorn precis efter first är last, så det finns bara ett element
    if (std::next(first) == last) {
		//om det elementet har egenskapen p så returnerar vi last, annars returnerar vi first
        if (p(*first)) {
            return last;
        }
		//om det inte har egenskapen p så returnerar vi first
        else {
            return first;
        }
    }

	// Divide to get the mid value, beräknar mitten av intervallet [first, last) genom att ta avståndet mellan first och last och dela det på 2, sedan addera det till first
	auto mid = first + std::distance(first, last) / 2;  //distance(first, last) ger antalet element i intervallet [first, last), och genom att dela det på 2 får vi antalet element i den första halvan av intervallet. Genom att addera det till first får vi iteratorn som pekar på mitten av intervallet.

    // Recursively solve left and right halves
	auto it1 = TND004::stable_partition(first, mid, p); //vänstra halvan, returnerar iteratorn som pekar på slutet av blocket med egenskapen p i den vänstra halvan
    auto it3 = TND004::stable_partition(mid, last, p); //högra halvan, returnerar iteratorn som pekar på slutet av blocket med egenskapen p i den högra halvan

    // Conquer
    return std::rotate(it1, mid, it3);
}
