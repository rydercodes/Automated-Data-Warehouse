#include <bits/stdc++.h>

using namespace std ;



const int MAXTABLES        = 11; // maximum number of tables you could raise it up.
const int SQLKEYWORDCOUNT  = 37; // number of sql keywords
const int TERMINALMAXCOL   = 62; // maximum columns in a single row in terminal

class Date {
   public: 
   int day  ;
   int month;
   int year ;

   Date() { 
      time_t now  = time(0);
      tm *ltm     = localtime(&now);

      day   = ltm->tm_mday;
      month = ltm->tm_mon ;
      year  = ltm->tm_year + 1900;
   }

   Date(int m, int d, int y){
      month = m; 
      day   = d; 
      year  = y;
   }

   friend ostream& operator<<(ostream& os, const Date& dt){ 
      os << dt.year << '/' << dt.month << '/' << dt.day;
      return os;
   }

   friend istream& operator>> (istream& is, Date& dt) 
   {
      is>> dt.year >> dt.month>> dt.day;
      return is;
   }

};

class Product {
   public:
   string product ;
   string category;

   Product() {
      product  = "";
      category = "";
   }

   Product(string prdct, string ctgry) {
      product  = prdct;
      category = ctgry; 
   }

   friend ostream & operator << (ostream & os, const Product & p) {
      os << p.product << ' ' << p.category << endl;
      return os;
   }

   friend istream & operator >> (istream & is, Product & p) {
      is >> p.product >> p.category ;
      return is;
   }
};

class Location {
   public:
   string shop;
   string city ;
   string region;

   Location() {
      shop     = "";
      city     = "Venice";
      region   = "Europe";
   }

   Location(string shp, string cty, string rgn) {
      shop     = shp;
      city     = cty;
      region   = rgn;
   }

   friend ostream & operator << (ostream & os, const Location & l) {
      os << l.shop << ' ' << l.city << ' ' << l.region << endl;
      return os;
   }

   friend istream & operator >> (istream & is, Location & l) {
      is >> l.shop >> l.city >> l.region ;
      return is;
   }

};

class Transactions {
   public:
   string   sales;
   int      productId;
   string   shop;
   int      dateId ;

   Transactions() {
      sales       = "";
      productId   = 1;
      shop        = "";
      dateId      = 1;
   }

   Transactions(string sls, int prdctId, string shp, int dateId) {
      sales       = sls;
      productId   = prdctId;
      shop        = shp;
      dateId      = dateId;
   }

   friend ostream & operator << (ostream &os, const Transactions &t) {
      os << t.sales << ' ' << t.shop << endl;
      return os;
   }

   friend istream & operator >> (istream & is, Transactions & t) {
      is >> t.sales >> t.productId >> t.shop >> t.dateId ;
      return is;
   }
};

class Input {
   public:

   int tableCount ;

   vector   <string> tables;
   map      <string, int> indexes ;
   vector   <string> adj[MAXTABLES];

   void Introduce() {
      cout << "Hello \nThis is a final project for AA2\nPlease follow the instruction:\n";
      cout << "1-Please Enter the number of tables" << endl;
      cout << "2-Please Enter names for each table follow by their columns\n";
      cout << "3-At the End please enter the desired input to get your query!" << endl;
      for (int ind = 0 ; ind < TERMINALMAXCOL ; ++ind) cout << "-";
      cout << endl;
   }

   void GetInput(){
      cout << "Please Enter number of tables : \n";
      cin >> tableCount ;

      for(int ind = 0 ; ind < tableCount ; ++ind) {
         cout << "Well Please Enter Table Name : \n" ;
         string table ; cin >> table;
         tables.push_back(table);

         indexes[table] = ind+1;

         cout << "Please Enter count of column's " + table << endl;
         int columnsCount ; cin >> columnsCount;
         for(int columndId = 0 ; columndId < columnsCount ; columndId++) {
            cout << "Enter column " << columndId+1 << " : " ;
            string column ; cin >> column ;
            adj[ind].push_back(column);
         }
         for(int i = 0 ; i < TERMINALMAXCOL ; ++i) cout << "-" ;
         cout << endl;
      }
   }

   friend ostream & operator << (ostream &os, Input &i) {
      return os;
   }
};

class Query {
   public:

   Input in ;
   bool  visited[MAXTABLES]; 

   static const string keywords[]; 


   Query(Input i) {
      in = i;
   }

   string toLowerString(string q) {
      string query = q;
      for (int ind = 0 ; ind < query.size() ; ind ++) {
            query[ind] = tolower(query[ind]);
      }
      return query;
   }

   void DFS(int u) { 
      visited[u] = true;
      for(int ind = 0 ; ind < in.adj[u].size() ; ind ++) {
         int v = in.indexes[in.tables[ind]];
         if (!visited[v]) {
            DFS(v);
         }
      }
   }

   bool isSpecialKeyword(string query) {
      for (int ind = 0 ; ind < SQLKEYWORDCOUNT ; ind++) {
         string key = toLowerString(keywords[ind]);
         query = toLowerString(query);
         if(key == query) {
            return true;
         }
      }
      return false;  
   }

   int findTableIndex(string q) {
      string query = toLowerString(q);
      for (int ind = 0 ; ind < in.tables.size() ; ind++) {
         if (query == in.tables[ind]) {
            // index of table
            return ind;
         }
      }
      return -1;
   }

   pair<string, string> findTableColumn(string q) {
      string query = toLowerString(q);

      for (int ind = 0 ; ind < in.tables.size() ; ind++) {

         for(int columnId = 0 ; columnId < in.adj[ind].size() ; columnId++) {
            if (query == in.adj[ind][columnId]) {
               return make_pair(in.tables[ind],in.tables[ind] + "." + in.adj[ind][columnId]);
            }
         }
      }
      return make_pair("", "");
   }

   vector <string> splitQuery(string input) {
      istringstream iss(input);
      vector<string> results((istream_iterator<string>(iss)), istream_iterator<std::string>());
      return results;
   }

   string replaceToSpace(string input) {
      string in = input;
      for(int ind = 0 ; ind < in.size() ; ind ++) {
         if(in[ind] == ','){
            in[ind] = ' ';
         }
      }
      return in;
   }

   string findSubQuery(vector <string> tableToAdd) {
      int numberOfEquality = tableToAdd.size()-1;
      string ret = "";
      for(int tableInd = 1 ; tableInd < tableToAdd.size() ; tableInd++) {
         for(int columnId = 0 ; columnId < in.adj[in.indexes[tableToAdd[tableInd]]-1].size() ; columnId ++ ) {
            for(int firstTableInd = 0 ; firstTableInd < in.adj[in.indexes[tableToAdd[0]]-1].size() ; firstTableInd++) {
               if(in.adj[in.indexes[tableToAdd[0]]-1][firstTableInd] == in.adj[in.indexes[tableToAdd[tableInd]]-1][columnId]) {
                  ret += tableToAdd[0] + "." + in.adj[in.indexes[tableToAdd[0]]-1][firstTableInd];
                  ret += "=";
                  ret += tableToAdd[tableInd] + "." + in.adj[in.indexes[tableToAdd[tableInd]]-1][columnId];
                  if (numberOfEquality > 1) {
                     ret += ",";
                  }
                  numberOfEquality -- ;
                  if (numberOfEquality == 0) {
                     return ret ;
                  }
                  break;
               }
            }
         }
      }
      return ret;
   }

   string addSelect() {
      return "SELECT ";
   }

   string addFrom() {
      return " FROM ";
   }

   string addWhere() {
      return " WHERE ";
   }

   string addSemicolon() {
      return ";";
   }

   string makeQuery(string input) {
      string ret = addSelect();

      input = replaceToSpace(input); // first thing is we need to parse input into list of string 
      vector <string> splits = splitQuery(input) ; // we need to split each key into a vector
      if (splits.size() < 1) { // if size of splits vector is less than 1 then input is invalid !
         return "input is invalid, please enter a valid input!";
      }

      int mathKeywordInd = -1;
      string mathKeywords[] = {"AVG", "MAX", "MIN", "SUM"};
      for (int ind = 0 ; ind < 4 ; ++ind) {
         if(mathKeywords[ind] == splits[0]) {
            mathKeywordInd = ind ;
            break;
         }
      }

      bool isFirstMath = false; // if isFirstMath is true then the first keyword in input is a sql keyword !
      int wordInd = 0;
      vector <string> tableToAdd ; // list of tables which program want to process on it

      if (mathKeywordInd > -1) {
         ret         += mathKeywords[mathKeywordInd] ;
         isFirstMath = true;
      }
      else {
         ret                        += "SUM" ;
         ret                        += "(";
         pair<string, string> temp  = findTableColumn(splits[0]);
         tableToAdd.push_back(temp.first);
         ret += temp.second;
         ret += "),";
         wordInd ++ ;
      }

      if (isFirstMath) {
         wordInd = 2 ;
         ret += "(";
         pair<string, string> temp = findTableColumn(splits[1]);
         tableToAdd.push_back(temp.first);
         ret += "),";
      }
      
      for (; wordInd < splits.size() ; wordInd++) {
         ret += "(";
         pair<string, string> temp = findTableColumn(splits[wordInd]);
         ret += temp.second;
         tableToAdd.push_back(temp.first);
         ret += ")";
         if (wordInd < splits.size()-1 ) {
            ret += ",";
         }
      }

      ret += addFrom(); // add FROM keyword to query
      

      for(int tableInd = 0 ; tableInd < tableToAdd.size() ; tableInd ++) {
         ret += tableToAdd[tableInd];
         
         if (tableInd < tableToAdd.size()-1) {
            ret += ", ";
         }
      }

      ret += addWhere(); // add WHERE keyword to query
      ret += findSubQuery(tableToAdd) ; // find condition of where statement !
      ret += addSemicolon() ; // add semicolon to query

      return ret;
   }

   friend ostream &  operator << (ostream &os, Query &q) {
      return os;
   }

};

// list of all keywords in sql

const string Query::keywords [] = {
   "CREATE",   "PRIMARY KEY",   "INSERT",   "INTO",   "SELECT",   "FROM",   "ALTER",   "DROP",
   "ADD",   "DISTINCT",   "UPDATE",   "SET",   "DELETE",   "TRUNCATE",   "AS",   "ORDER BY",
   "ASC",   "DESC",   "BETWEEN",   "WHERE",   "AND",   "OR",   "NOT",   "LIMIT",   "IS NULL",
   "DROP COLUMN",   "DROP DATABASE",   "DROP TABLE",   "GROUP BY",   "HAVING",   "IN",   "JOIN",
   "UNION",   "UNION ALL",   "EXISTS",   "LIKE",   "CASE"
};


int main() {
 
 //  please make sure put your input file in same directory !
   freopen("input.txt", "r", stdin);
   Input in ; 

   in.Introduce(); 
   in.GetInput();

   cout << "Please Enter input to generate query : " << endl;
   string input ;
   getline(cin, input);
   getline(cin, input);

   cout << input << endl;

   Query q (in);
   string ans = q.makeQuery(input);
   cout << ans << endl;

   return 0;
}