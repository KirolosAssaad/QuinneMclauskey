#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<cmath>
#include<math.h>
using namespace std;

vector<string> temp; //global vector of strings
int counterCorner = 0;//global int used to detect corner cases


vector<string> fetchFromFile(int& numberOfVariables, vector<string>& all, vector<string>& minTermsInDecimal, vector<string>& DCTInDecimal)
//returns the number of variables and a vector containing the minterms and another containing don't care terms as
// and a vector containing both of them in decimals
{
	ifstream in;
	in.open("input.txt"); //name of file containing input

	string temp1, temp2;

	if (in.is_open())
	{
		in >> numberOfVariables;
		in >> temp1;//minterms in decimal
		in >> temp2;//don't care terms in decimal
	}
	else
	{
		cout << "Opening input file failed " << endl;
		exit(0);
	}

	if ((numberOfVariables < 1) || (numberOfVariables > 23))
	{
		cout << "Number of variables inputted is not supported by the program" << endl;
		cout << "Please edit your input file to a number of variables between 1 and 23" << endl;
		exit(0);
	}
	if ((temp1 == ",!") || (temp1 == ""))
	{
		cout << "Since no minterms are inputted the output is always 0";
		exit(0);
	}



	while (temp1.size() > 0)
	{
		int temp = temp1.find(',');
		minTermsInDecimal.push_back(temp1.substr(0, temp));
		all.push_back(temp1.substr(0, temp));
		temp1.erase(0, temp + 1);
		if (temp1[0] == '!')
		{
			minTermsInDecimal.push_back(temp1.substr(1, temp1.size() - 1));
			all.push_back(temp1.substr(1, temp1.size() - 1));
			temp1.clear();
		}
	}

	while (temp2.size() > 0)
	{
		int temp = temp2.find(',');
		DCTInDecimal.push_back(temp2.substr(0, temp));
		all.push_back(temp2.substr(0, temp));
		temp2.erase(0, temp + 1);
		if (temp2[0] == '!')
		{
			DCTInDecimal.push_back(temp2.substr(1, temp1.size() - 1));
			all.push_back(temp2.substr(1, temp1.size() - 1));
			temp2.clear();
		}
	}


	for (int i = 0; i < all.size(); i++)
	{
		if (all[i] == "")
			all.erase(all.begin() + i);
	}
	for (int i = 0; i < minTermsInDecimal.size(); i++)
	{
		if (minTermsInDecimal[i] == "")
			minTermsInDecimal.erase(minTermsInDecimal.begin() + i);
	}
	for (int i = 0; i < DCTInDecimal.size(); i++)
	{
		if (DCTInDecimal[i] == "")
			DCTInDecimal.erase(DCTInDecimal.begin() + i);
	}



	bool flagError = false;

	for (int i = 0; i < all.size(); i++)
		if (stoi(all[i]) > pow(2.0, numberOfVariables))
			flagError = true;
	if (flagError)
	{
		cout << "Minterms or Dont care terms have an invalid term, exiting now... " << endl;
		exit(0);
	}




	return all;
}

string decToBinInterior(const string& n)
{//This function converts its parameter from decimal to binary and returns it
	int num = stoi(n);
	vector<int> a;
	int i;
	for (i = 0; num > 0; i++)
	{
		a.push_back(num % 2);
		num = num / 2;
	}
	string result;
	for (i = i - 1; i >= 0; i--)
	{
		if (a[i])
			result.push_back('1');
		else
			result.push_back('0');
	}

	return result;
}

void decimalToBinary(vector<string>& v)
{//This function calls on the previous function ( decToBinInterior) to loop over the vector needed to be converted
	vector<string> result;
	for (int i = 0; i < v.size(); i++)
	{
		if (stoi(v[i]) == 0)
			result.push_back("0");
		else
			result.push_back(decToBinInterior(v[i]));
	}
	v = result;
}

void padding(vector<string>& v, int n)
{//This function adds zeros to the beginning of a function so that all
// the minterms and don’t care terms are of the same size, which is the variable size. 
//(e.g.: 10 becomes 0010 if the number of variables inputted in 4)
	for (int i = 0; i < v.size(); i++)
		if (v[i].size() < n)
		{
			string temp = v[i];
			for (int j = v[i].size(); j < n; j++)
				temp = '0' + temp;
			v[i] = temp;
		}
}

void printVector(const vector<string>& v)
{//This function takes a vector of strings and prints out all the terms it is carrying.
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;
	cout << endl;
}

void printDoubleVector(const vector<double>& v)
{//This function takes a vector of doubles and prints out all the terms it is carrying.
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;
	cout << endl;
}

void removeRedundancy(vector<string>& v)
{//This function loops over a vector of strings and removes any duplicate in it.
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = i + 1; j < v.size(); j++)
		{
			if (v[i] == v[j])
			{
				v.erase(v.begin() + j);
			}
		}
	}
}

void removeRedundancyDecimal(vector<double>& v)
{//This function loops over a vector of doubles and removes any duplicates in it.
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = i + 1; j < v.size(); j++)
		{
			if (v[i] == v[j])
			{
				v.erase(v.begin() + j);
			}
		}
	}
}

bool checkDiff(string a, string b, vector<string>& v, vector<string>& tableTerms)
{//This function checks the difference between two strings and if the number of differences is one, it is replaced with a dash.
	int count = 0; //keeps track of number of differences
	int position;  //keeps track of different element's index
	bool flag = false;//return value of function
	for (int i = 0; i < a.size(); i++)
	{
		if (a[i] != b[i]) // if char is different
		{
			count++; //increment count
			position = i; //store position of this char

		}
	}
	if (count == 1) // if one difference only
	{
		if ((a[position] != '-') && (b[position] != '-')) // and none of the values is a dash
		{
			flag = true; // found successfully
			a[position] = '-'; // dash the different char
			v.push_back(a); // store it in v
		}
	}
	return flag;
}

void getIteration(vector<string>& v, vector<string>& tableTerms, vector<string>& primeImplicants)
{//This function pushes back the ith prime implicant.
	vector<bool> isReduced;
	for (int i = 0; i < tableTerms.size(); i++)
		isReduced.push_back(false); // set all terms as not reduced

	for (int i = 0; i < tableTerms.size(); i++)
	{
		for (int j = i + 1; j < tableTerms.size(); j++)
		{
			if (checkDiff(tableTerms[i], tableTerms[j], v, tableTerms)) // add in v the current column
			{


				isReduced[i] = true;// node is reduced thus not prime implicant
				isReduced[j] = true;// node is reduced thus not prime implicant
				removeRedundancy(v);
				removeRedundancy(tableTerms);
			}
		}
	}

	for (int i = 0; i < tableTerms.size(); i++)//here we reached end of ith column
	{
		if (!isReduced[i])//element not reduced
			primeImplicants.push_back(tableTerms[i]);//must be a prime implicant
	}
	tableTerms = v;
	counterCorner++;
	v.clear();
}

void primeImplicantTableGenerator(vector<string>& v, vector<string>& tableTerms, vector<string>& primeImplicants)
{//This function loops on the iteration function to get the whole list of prime implicants.
	do
	{
		getIteration(v, tableTerms, primeImplicants);
	} while (v != tableTerms);
}

vector<string> EPI_min(vector<string> table, int var, vector<string> list, vector<string> dc)
{//This function gives out the minterms of all the essential prime implicants.
	vector<string> essential;

	for (int i = 0; i < table.size(); i++)
	{
		bool flag = false;
		string temp1 = table[i];
		string temp2 = table[i];
		for (int j = 0; j < var; j++)
		{
			if (table[i][j] == '-')
			{
				temp1[j] = '0';
				temp2[j] = '1';
				flag = true;
				table.push_back(temp1);
				table.push_back(temp2);
			}

			if (flag)
				break;
		}
	}

	for (int j = 0; j < table.size(); j++)
	{
		bool flag = false;
		for (int k = 0; k < var; k++)
		{
			if (table[j][k] == '-')
			{
				flag = true;
			}

			if (flag)
				break;
		}
		if (!flag)
			list.push_back(table[j]);
	}
	for (int i = 0; i < list.size(); i++)
	{
		bool check = false;
		for (int j = i + 1; j < list.size(); j++)
		{
			if (list[i] == list[j])
			{
				check = true;
				list.erase(list.begin() + j);
			}
		}
		if (!check)
			essential.push_back(list[i]);
	}

	for (int i = 0; i < essential.size(); i++)
	{
		for (int j = 0; j < dc.size(); j++)
		{
			if (essential[i] == dc[j])
			{
				essential.erase(essential.begin() + i);
				i = 0;
			}
		}
	}
	return essential;
}

void possibility(string a, int var, vector<string>& min, vector<string>& essential)
{//This function checks and pushes back one essential prime implicant if found at string a.

	vector<string>table;
	table.push_back(a);
	for (int i = 0; i < table.size(); i++)
	{
		string temp1 = table[i];
		string temp2 = table[i];
		bool flag = false;
		for (int j = 0; j < var; j++)
		{
			if (table[i][j] == '-')
			{
				temp1[j] = '0';
				temp2[j] = '1';
				flag = true;
				table.push_back(temp1);
				table.push_back(temp2);
			}
			if (flag)
				break;
		}
	}
	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < min.size(); j++)
		{
			if (table[i] == min[j])
				essential.push_back(table[0]);
		}
	}
	removeRedundancy(essential);
}

void EPI(int var, vector<string>& min, vector<string>& essential, vector<string> table)
{//This function loops over the previous function getting the list of all essential prime implicants.
	for (int i = 0; i < table.size(); i++) {
		possibility(table[i], var, min, essential);
	}
}

vector<double> non_used(vector<string> min, int var, vector<string>dc)
{//This function returns the decimal of all the minterms that have not been used due to not being essential.
	vector<string> list2;
	vector<double> min_dec;
	for (int i = 0; i < temp.size(); i++)
	{
		bool flag = false;
		for (int j = 0; j < min.size(); j++)
		{
			if (min[j] == temp[i])
				flag = true;
			if (flag)
				break;
		}
		if (!flag)
			list2.push_back(temp[i]);
	}
	for (int i = 0; i < list2.size(); i++)
	{
		for (int j = 0; j < dc.size(); j++)
		{
			if (list2[i] == dc[j])
			{
				list2.erase(list2.begin() + i);
			}
		}
	}
	for (int i = 0; i < list2.size(); i++)
	{
		double temp1 = 0;
		for (int j = 0; j < var; j++) {
			double power = 0;
			if (list2[i][j] == '1') {
				power = var - j - 1;
				{if (power == 0)
					temp1 = temp1 + 1;
				else
					temp1 = temp1 + pow(2.0, power);
				}
			}
		}
		min_dec.push_back(temp1);
	}
	return min_dec;
}


vector<string> bool_expression(int var, vector<string>& essentials)
{//This function takes the essential prime implicants and returns the Boolean expression of it.
	vector<string> bool_exp;
	char characters[23] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w' };
	for (int i = 0; i < essentials.size(); i++) {
		string exp;
		for (int j = 0; j < var; j++)
		{
			if (essentials[i][j] == '0')
			{
				exp.push_back(characters[j]);
				exp.push_back(39);
			}
			else if (essentials[i][j] == '1')
				exp.push_back(characters[j]);
		}
		bool_exp.push_back(exp);
	}
	removeRedundancy(bool_exp);
	return bool_exp;
}
void print_all_PI(vector<string>primeImplicants, vector<string> min, vector<string> dc, int var)
{//This function prints the binary representation of the prime implicants as well as the minterms and the don’t care terms used in them.
	vector<string> list;
	vector<string> table;
	vector<string> mins;
	vector<double> min_dec;

	vector<string> DCs;
	vector<double> dc_dec;

	for (int s = 0; s < primeImplicants.size(); s++) {
		table.push_back(primeImplicants[s]);
		for (int i = 0; i < table.size(); i++)
		{
			bool flag = false;
			string temp1 = table[i];
			string temp2 = table[i];
			for (int j = 0; j < var; j++)
			{
				if (table[i][j] == '-')
				{
					temp1[j] = '0';
					temp2[j] = '1';
					flag = true;
					table.push_back(temp1);
					table.push_back(temp2);
				}

				if (flag)
					break;
			}
		}

		for (int j = 0; j < table.size(); j++)
		{
			bool flag = false;
			for (int k = 0; k < var; k++)
			{
				if (table[j][k] == '-')
				{
					flag = true;
				}

				if (flag)
					break;
			}
			if (!flag)
				list.push_back(table[j]);
		}
		cout << endl;
		for (int k = 0; k < list.size(); k++) {
			bool flag = false;
			for (int a = 0; a < min.size(); a++) {
				if (list[k] == min[a])
				{
					mins.push_back(list[k]);
					flag = true;
					if (flag) break;
				}
			}
		}
		for (int k = 0; k < list.size(); k++) {
			bool flag = false;
			for (int a = 0; a < dc.size(); a++) {
				if (list[k] == dc[a])
				{
					DCs.push_back(list[k]);
					flag = true;
					if (flag) break;
				}
			}
		}








		cout << primeImplicants[s] << "   ";
		bool min_check = true;
		if (mins.size() == 0)
			min_check = false;
		bool dc_check = true;
		if (DCs.size() == 0)
			dc_check = false;





		removeRedundancy(dc);
		removeRedundancy(mins);


		for (int i = 0; i < mins.size(); i++)
		{
			double temp1 = 0;
			for (int j = 0; j < var; j++) {
				double power = 0;
				if (mins[i][j] == '1') {
					power = var - j - 1;
					{if (power == 0)
						temp1 = temp1 + 1;
					else
						temp1 = temp1 + pow(2.0, power);
					}
				}
			}
			min_dec.push_back(temp1);
		}
		for (int i = 0; i < DCs.size(); i++)
		{
			double temp1 = 0;
			for (int j = 0; j < var; j++) {
				double power = 0;
				if (DCs[i][j] == '1') {
					power = var - j - 1;
					{if (power == 0)
						temp1 = temp1 + 1;
					else
						temp1 = temp1 + pow(2.0, power);
					}
				}
			}
			dc_dec.push_back(temp1);
		}


		removeRedundancyDecimal(min_dec);
		removeRedundancyDecimal(dc_dec);


		if (min_check) cout << "minterms: (";


		for (int d = 0; d < mins.size(); d++) {
			if (!min_check) break;
			if (d < mins.size() - 1)
				cout << mins[d] << ",";
			else cout << mins[d] << ") ";
		}
		if (dc_check) cout << "don't cares: (";

		for (int d = 0; d < DCs.size(); d++) {
			if (!dc_check) break;
			if (d < DCs.size() - 1)
				cout << DCs[d] << ",";
			else cout << DCs[d] << ")";
		}
		cout << " and in decimal: ";
		if (min_check) cout << "minterms: (";


		for (int d = 0; d < min_dec.size(); d++) {
			if (!min_check) break;
			if (d < min_dec.size() - 1)
				cout << min_dec[d] << ",";
			else cout << min_dec[d] << ") ";
		}
		if (dc_check) cout << "don't cares: (";

		for (int d = 0; d < dc_dec.size(); d++) {
			if (!dc_check) break;
			if (d < dc_dec.size() - 1)
				cout << dc_dec[d] << ",";
			else cout << dc_dec[d] << ")";
		}
		table.clear();
		list.clear();
		mins.clear();
		DCs.clear();
		min_dec.clear();
		dc_dec.clear();
	}


}




bool all_ones_counter(vector<string> primeImplicants, int var)
{//This function checks the corner case of all the minterms and don’t care terms are one which means 
	//the final prime implicant is just dashes and only returns one.
	int counter = 0;
	bool allOneCorner = false;
	if (primeImplicants.size() == 1) {
		for (int i = 0; i < var; i++) {
			if (primeImplicants[0][i] == '-')
				counter++;
		}
	}
	if (counter == var)
		allOneCorner = true;


	return allOneCorner;
}

void runProgram()
{//This function runs the whole program and is called at in the int main function.
	int noVariables;

	vector<string> allTerms;
	vector<string> minTerms;
	vector<string> dCTerms;

	fetchFromFile(noVariables, allTerms, minTerms, dCTerms);

	decimalToBinary(allTerms);
	decimalToBinary(minTerms);
	decimalToBinary(dCTerms);

	padding(allTerms, noVariables);
	padding(minTerms, noVariables);
	padding(dCTerms, noVariables);

	temp = allTerms;

	vector<string> primeImplicants;
	vector<string> v;
	vector<string> list;
	vector<string> min;
	vector<string> essential;
	vector<double> list2;
	vector<string> final;
	vector<string> dc;

	primeImplicantTableGenerator(v, allTerms, primeImplicants);


	if (all_ones_counter(primeImplicants, noVariables))
	{
		cout << "since all possible inputs are of values 1, the final output will be 1" << endl;
		exit(0);
	}

	print_all_PI(primeImplicants, minTerms, dCTerms, noVariables);


	if (counterCorner == 1)
		primeImplicants = temp;



	cout << endl;
	min = EPI_min(primeImplicants, noVariables, list, dCTerms);

	vector<string> min2;



	EPI(noVariables, min, essential, primeImplicants);





	final = bool_expression(noVariables, essential);

	list2 = non_used(min, noVariables, dCTerms);


	cout << "---------------------------------------------------------------" << endl;
	cout << "List of essential prime implicants is: " << endl;
	printVector(final);


	cout << "---------------------------------------------------------------" << endl;
	cout << "Minterms that are not covered by the essential PIs: " << endl;
	printDoubleVector(list2);

}

int main()
{
	runProgram();
	return 0;
}