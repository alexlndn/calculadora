#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cctype>
#include <fstream>

//comentar que como es una lista de string, tengo que redefinir el nodo
//resaltar que solo se cambia la parte int de la lista anterior por string
using namespace std;


class Token{
  protected:
    int numero;
    char caracter;
    string type;
    
  public:
    Token(){};
    Token(int n){
      numero = n;
      type="number";
    };
    Token(char c){
      caracter = c;
      type="char";
    };
    string getType(){
      return type;
    }
    int getNumero(){
      return numero;
    }
    char getChar(){
      return caracter;
    }
};

class Nodo{
  protected: 
    Token *dato;
    Nodo *next;
  public:
    Nodo(){next=NULL;};
    Nodo(Token *a) {dato=a; next=NULL;};
    void set_dato(Token *a) {dato=a; };
    void set_next(Nodo *n) {next=n; };
    Token *get_dato() {return dato; };
    Nodo *get_next() {return next; };
    bool es_vacio() {return next==NULL;}
};

class Lista{
	protected: 
		Nodo *czo;
	public:
		Lista(){czo=new Nodo();};
		Lista(Nodo *n) {czo=n;};
		//~Lista(void);
		//tipolista Lista::ultimo(tipolista x);
		void del(void);
		void add(Token *d);
		bool esvacia(void);
		Token *cabeza(void);
		Lista *resto(void);
		string toPrint(string p);    
};

class Pila:public Lista{
	public:
		Pila(){Lista();};
		void apilar(Token *x){add(x);};
		Token *tope(void){return cabeza();};
		void desapilar(void){del();};
		bool pilavacia(){return esvacia();};
};                  

void Lista::del(void){ 
	Nodo *aux;
	aux=czo;
	czo=czo->get_next();
	delete aux;
}
void Lista::add(Token *d){  
	Nodo *nuevo=new Nodo(d);
	nuevo->set_next(czo);
	czo=nuevo;
}
bool Lista::esvacia(void){   
	return czo->es_vacio();
}

Token *Lista::cabeza(void){ 
  if(esvacia()){
    cout<<" Error, Cabeza de lista vacia\n\n";
    return new Token(); 
  }
  return czo->get_dato();
}

Lista *Lista::resto(void){ 
  Lista *l=new Lista(czo->get_next());
  return (l);
}

string Lista::toPrint(string p){
	if (this->esvacia()) {
		return p;
	} else {
		std::ostringstream stm;
		if(this->cabeza()->getType() == "number")
			stm << this->cabeza()->getNumero() << "-" << this->resto()->toPrint(p) << endl;
		else
			stm << this->cabeza()->getChar() << "-" << this->resto()->toPrint(p) << endl;
		return stm.str();
	}
}

class Calculadora{
	protected:
		bool ok;
		Pila *pilaOriginal;
		bool isnum(char valor);
		bool parentesis(string cad);
		bool logica(string cad);
		bool analizar(string cad);
		Pila* invertirPila(Pila *original);
		void operar(Pila *p, int a);
		
	public:
		Calculadora(string cad){ ok = analizar(cad); };
		int calcular(Pila *p);
		void tryCalcular(Pila *p);
};

/**
  * Determina si el caracter recibido es un n�mero
  * @param valor El caracter a analizar
  * @return true si el valor es un n�mero
  */
  
//Comprueba si el char enviado es un numero o no, utilizando los metodos de "cctype"
bool Calculadora::isnum(char valor){ 
	if(!isalpha(valor) && isalnum(valor)) //Comprueba si no es una letra y si es un caracter alfanumerico, por ende, si es un numero
		return true;	
	return false; 
};


Pila* Calculadora::invertirPila(Pila *original){
	Pila *p = new Pila();
	bool seguir = true;
	while(seguir){
		p->apilar(original->tope());
		original->desapilar();
		if(original->pilavacia())
			seguir = false;
	}
	return p;
}

// Analiza parentesis: tanto su cantidad como el orden
bool Calculadora::parentesis(string cad){
	bool correcto= true;
	Pila *parentesis = new Pila(); 
	//Ciclo for a lo largo de cadena, donde apila los parentesis abiertos cuando los detecta y comprueba la pila cuando se cierran
	for(int i = 0 ; i < cad.length() && correcto ; i++){	
		
		if(cad.at(i)=='(')				
			parentesis->apilar(new Token('('));
		else if(cad.at(i)==')')			
			if(parentesis->pilavacia())
				correcto = false;
		else
			parentesis->desapilar();
	}
	if(!parentesis->pilavacia())		//Si la pila no es vacia significa que quedo un parentesis sin cerrar y por ende la expresion estaba mal.
		correcto = false;
	return correcto;					//Si la pila es vacia retorna la variable "correcto" que sera false o true dependiendo de que sucedio en el ciclo for.
}

//Analiza orden de simbolos, numeros y parentesis (que la logica sea correcta para utilizarlo en calculadora).
bool Calculadora::logica(string cad){
	bool correcto = true;
	string valor;					//En esta variable guardamos el ultimo tipo de valor que leimos (num,signo,(,) )
	if(isnum(cad.at(0)))
		valor = "num";
	else if(cad.at(0)=='(' && cad.length() > 2) 
			valor = "(";
	else{						//Si el primer valor no es parentesis abierto ni numero entonces la exprecion esta mal y cambia el valor de "correcto" a false
		correcto = false;		 
		}
	//Comprueba los demas valores (desde el segundo hasta el anteultimo). Si detecta un error, cambia el valor de la variable "correcto" a false y no vuelve a entrar al ciclo
	for(int i = 1 ; i < (cad.length() -1) && correcto ; i++){
		if(cad.at(i)=='(')
			if(valor != "signo" && valor != "(")
				correcto = false;
			else
				valor = "(";
		else if(cad.at(i)==')')
			if(valor != "num" && valor != ")")
				correcto = false;
			else
				valor = ")";
		else if(cad.at(i)=='*' || cad.at(i)=='/' || cad.at(i)=='+')
			if(valor!= "num" && valor!= ")")
				correcto = false;
			else
				valor = "signo";
		else if(cad.at(i)=='-')
			if(valor!= "num" && valor!= ")" && valor!= "(")
				correcto = false;
			else
				valor = "signo";
		else if(isnum(cad.at(i)))
			if(valor == ")")
				correcto = false;
			else
				valor = "num";
		else correcto = false;		//Si el valor no ingresa en ningun if, se establece correcto como falso
	}
	
	if(isnum(cad.at(cad.length() -1)))
		return correcto;
	else if(cad.at(cad.length() -1) ==')' && valor!="signo") 
		return correcto;
	
	correcto = false;						//Si ultimo el valor no es numero ni parentesis cerrado, entonces esta mal y cambia el valor de "correcto" a false y lo retorna
	return correcto;						
}


bool Calculadora::analizar(string cad){
	Pila *p = new Pila();
	int num;
	int temp;									
	bool cond;
	
	if(logica(cad) && parentesis(cad)){         //Comprueba que la cadena ingresada sea utilizable
	
		for(int i = 0 ; i < cad.length() ; i++){	//Carga la pila con los valores de la cadena
			if(isnum(cad.at(i)))
				if(p->pilavacia()){				//Si es un numero y la pila es vacia, entonces lo agrega directo, haciendo la conversion de char a int (char de interes - char '0')
					num = cad.at(i) -'0';
					p->apilar(new Token(num));
				}
				else if(p->tope()->getType()=="number"){	//Si es un numero y el tope de la pila es tambien numero entonces lo desapila y lo convierte en un unico numero (anterior*10 + nuevo) y lo guarda, siempre haciendo la convercion de char a int
					temp = p->tope()->getNumero();
					p->desapilar();
					num = cad.at(i) -'0';
					num = temp*10 + num;
					p->apilar(new Token(num));
				}
					else{									//Si es un numero y el anterior no es un numero, entonces lo agrega directo, haciendo la conversion de char a int (char de interes - char '0')
						num = cad.at(i) -'0';
						p->apilar(new Token(num));
					}
			else											//Si no es un numero lo apila directamente en un Token char
				p->apilar(new Token(cad.at(i)));
		}
		pilaOriginal = invertirPila(p);	
		cond = true;
	}
	else{
		cond = false;
	}
	return cond;
}

//Recibe una pila vacia que utilizara para hacer las operaciones sacada de la "pilaoriginal"
int Calculadora::calcular(Pila *p){
	Token val= *pilaOriginal->tope();
	if(val.getType() == "number"){
		operar(p,val.getNumero());
		pilaOriginal->desapilar();
	}else if(val.getChar() == '+' || val.getChar() == '-' || val.getChar() == '*' || val.getChar() == '/'){
		pilaOriginal->desapilar();
		p->apilar(&val);
	}else if( val.getChar() == '('){
		pilaOriginal->desapilar(); //elimina el parentesis abierto
		int valorSiguiente = calcular(new Pila());
		operar(p,valorSiguiente); // esto retorna un entero hay que hacer las operaciones necesarias
	}else if(val.getChar() == ')'){
		pilaOriginal->desapilar(); //elimina el parentesis cerrado
		return p->tope()->getNumero();
	}
	if(! pilaOriginal->pilavacia() ){
	  calcular(p);
  	}else{
		return p->tope()->getNumero();
  	}
  	return p->tope()->getNumero(); 
}

void Calculadora::tryCalcular(Pila *p){
	if(ok!=0){
		cout << "Resultado: " << calcular(p) << endl << endl;	
	}
	else{
		cout << "Error en la cadena.\n\n";
	}
}

void Calculadora::operar(Pila *p, int a){
	int x;
	if(p->pilavacia()){
		x = a;
	}else{
		// si tiene un signo en el tope (lo que necesariamente no significa que haya otro numero antes que el signo)
		// hay que ver si hay otro numero para hacer las operaciones, si no hay otro numero es porque es un signo menos
		Token *tok = p->tope(); //guardo el signo que hay en la pila
		p->desapilar(); //en este momento el p->tope() deberia tener un numero para operar
		if(p->pilavacia()){
		 if(tok->getChar() == '-')
			x = -a;
		 else
			x = a;
		}else{
			switch( tok->getChar()){
			case '+':
				x=p->tope()->getNumero() + a;
				break;
			case '-':
				x=p->tope()->getNumero() - a;
				break;
			case '*':
				x=p->tope()->getNumero() * a;
				break;
			case '/':
				x=p->tope()->getNumero() / a;
				break;
			}
			p->desapilar(); //desapila el numero que estaba
		}
	}
  p->apilar(new Token(x));
};

int main(){
  string cad;
  ifstream fInput;
  int opcion;
  char continuar = 's';
  
  while (continuar != 'n'){
  	cout << "Seleccione una opcion\n1: Ingresar cadena por teclado\n2: Leer fichero de texto\nOpcion ";
	  do{
	  	cin >> opcion;
		}while(opcion < 1 || opcion > 2);
		
		if(opcion == 1){
			cout << "Ingrese la cadena a resolver sin espacios: ";
	  	cin >> cad;
		}else{
			string archivo;
			cout << "Ingrese el nombre del archivo a leer (debe estar en la misma carpeta que el programa): ";
			cin >> archivo;
			archivo += ".txt";
			fInput.open(archivo.c_str());
			if(fInput.fail()){
				cout << "\nOcurrio un error al intentar abrir el archivo " << archivo << endl;
				return 0;
			}else{
				getline(fInput,cad);
				fInput.close();
			}
		}
	  Calculadora *c = new Calculadora(cad);
	  c->tryCalcular(new Pila());
	  free(c);
	  cout << "Continuar calculando? presione 'n' para cancelar...\n\n";
	  cin >> continuar;
  }
  system("PAUSE");
};

