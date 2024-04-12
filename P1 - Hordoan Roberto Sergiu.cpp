#include <iostream>
#include <fstream>
#include <vector> // we use vector because the limit values are not included, this means we can declare the vector with the size of a variable

using namespace std;

ifstream fin("input1.in");
ofstream fout("output1.txt");

unsigned long calculate_bell_number_n(int n) { // this is done with a time complexity of O(n^2) and a space complexity of O(n)
	/**
	* It calculates the bell number of order n using bell's triangle.
	* @param n - order of the bell number to calculate
	* @return the calculated bell number
	*/
	vector<unsigned long> current_line(n + 1, 0); // we can use just an array, as we can note that we do not necessarily have to use bidimensional structures
	unsigned long copy = 0; // we need copy to save temporary the element above in the bell's triangle
	unsigned long prev = 0; // prev is the previous element in the bell's triangle, which we do not have anymore, as we work on a unidimensional structure

	current_line[1] = 1; //for 1 element we have 1 partition

	for (int i = 2; i <= n; i++) {
		prev = current_line[1];
		current_line[1] = current_line[i - 1]; //we copy the last element of the last row as the first in the new
		for (int j = 2; j <= i; j++) {
			copy = current_line[j];
			current_line[j] = prev + current_line[j - 1]; //we add the element to the left with the element on top
			prev = copy;
		}
	}
	return current_line[n];
}

void print_number_of_partitions(int n, unsigned long number) {
	/**
	* It prints the number of partitions with the template provided in the pdf.
	* @param number - number of the partitions on the set A
	*		 n - number of elements of set A
	* @return print the output needed
	*/
	fout << "1. The number of partitions on a set A = {a1";
	for (int i = 2; i <= n; i++) {
		fout << "," << "a" << i;
	}
	fout << "} is " << number << '\n';
}

void print_equivalence_relation(int n, int partition[]) {
	/**
	* It generates and prints the equivalence relation of the partition printed before
	* @param n - number of elements of set A
	* partition - array representing current partition
	* @return print the output needed
	*/
	int c = 0;
	vector<vector<int>> relation(n + 1);
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= n; j++) {
			if (i != j && partition[i] == partition[j])
				c++, relation[i].push_back(j); //we add the element j to the vector that marks all of the elements to which i is in relation with
		}
	}
	if (c == n * (n - 1)) { //if we have the partition with one subset, we print A x A, as seen in the pdf
		fout << "A x A";
	}
	else {
		fout << "/" << char(92) << "A"; //i print char(92) instead of \ because my compiler thinks it is an escape character
		if (c == 0) //if it is the partition with n subsets, we do not need to print anything else
			return;
		fout << " U {";
		for (int i = 1; i <= n; i++) {
			for (int j = 0; j < relation[i].size(); j++) {
				c--;
				fout << "(a" << i << ",a" << relation[i][j] << ")";
				if (c)
					fout << ",";
			}
		}
		fout << "}";
	}
}

void print_partition(int n, int partition[]) {
	/**
	* It prints the current partition
	* @param n - number of elements of set A
	* partition - array representing current partition
	* @return print the output needed
	*/
	vector<vector<int>> partition_generated(n + 1); //we use a vector where we store the subsets of the partitions
	for (int i = 1; i <= n; i++) {
		partition_generated[partition[i]].push_back(i); //we add i to the vector of the subset to which it belongs
	}
	fout << "{";
	bool first = 1;
	for (int i = 1; i <= n; i++) {
		if (partition_generated[i].size() <= 0) //if this subset is empty, we print nothing
			continue;
		if (!first) //to not print commas more than we need
			fout << ",";
		first = 0;
		fout << "{";
		for (int j = 0; j < partition_generated[i].size(); j++) {
			fout << "a" << partition_generated[i][j];
			if (j < partition_generated[i].size() - 1)
				fout << ",";
		}
		fout << "}";
	}
	fout << "}";
	fout << " -> ";
	print_equivalence_relation(n, partition);
	fout << "\n";
}

void generate_partitions(int n, int partition[], int i = 1, int maximum = 0) {
	/**
	* It generates and prints all of the partitions on a set A with n elements
	* @param n - number of elements of set A
	* partition - array representing current partition
	* i - integer representing the current element we are adding to the partition, it is initialized when the function is first called with 1
	* maximum - integer representing the maximum partition we have assigned an element to, needed to not generate the same partition multiple times, it is initialized when the function is first called with 0
	* @return print the output needed
	*/
	if (i == n + 1)
		print_partition(n, partition);
	else {
		for (int j = 1; j <= i; j++) {
			partition[i] = j; // an element can be in only one subset at a time, as per the definition of partitions
			if (partition[i] - maximum > 1) //this is pretty interesting, basically if we leave an empty subset between two subsets, we then have multiple outputs that are the same 
				continue;
			generate_partitions(n, partition, i + 1, max(maximum, j));
		}
	}
}

void print_explanation(int n) {
	/**
	* It prints the output needed for the second statement
	* @param n - number of elements of set A
	* @return print the output needed
	*/
	fout << "2. Using the notation /" << char(92) << "A = {(a1,a1)"; //I use char(92) because my compiler thinks that \ is an escape character and it also does not print the character delta, as it is utf8
	for (int i = 2; i <= n; i++) {
		fout << ", (a" << i << ",a" << i << ")";
	}
	fout << "}, the partitions on a set A = {a1";
	for (int i = 2; i <= n; i++) {
		fout << ",a" << i;
	}
	fout << "} and their corresponding equivalence relations are:\n";
}

int main() {
	int n;
	fin >> n;
	unsigned long bell_number_n = calculate_bell_number_n(n); //unsigned long because bells number becomes very big after n > 10
	print_number_of_partitions(n, bell_number_n);
	int partition[9] = { 0 }; //we initialize with 0
	if (n <= 8) {
		print_explanation(n);
		generate_partitions(n, partition);
	}
	else
		fout << "2. N is bigger than 8!";
	fin.close(), fout.close(); //we close the folders to avoid any memory leaks
	return 0;
}

