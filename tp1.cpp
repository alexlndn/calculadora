//#include <cstdlib>
#include <iostream>
#include <sstream>
#include <cctype>


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

///////////////// CALCULADORA ///////////////////////

class Calculadora{
	protected:
		Pila *pilaOriginal;
		/**
		* Funcion encargada de procesar el entero recibido para operar con la pila
		*
		*/
		void operar(Pila *p, int a){
			int x;
			if(p->pilavacia()){
				x = a;
			}else if(p->tope()->getType()=="number"){
				x=p->tope()->getNumero()*10 + a;
				p->desapilar(); //desapilo el numero y llamo a la funcion de nuevo con el nuevo numero
				operar(p,x);
			}else{
				// si tiene un signo en el tope (lo que necesariamente no significa que haya otro numero antes que el signo)
				// hay que ver si hay otro numero para hacer las operaciones, si no hay otro numero es porque es un signo menos
				Token *tok = p->tope(); //guardo el signo que hay en la pila
				p->desapilar(); //en este momento el p->tope() deberia tener un numero para operar
				if(p->pilavacia()){
				 if(tok->getChar() == '-')
					x = -a;
				}else{
					switch( tok->getChar()){
					case '+':
//						p->desapilar();
						x=p->tope()->getNumero() + a;
						break;
					case '-':
//						p->desapilar();
						x=p->tope()->getNumero() - a;
						break;
					case '*':
//						p->desapilar();
						x=p->tope()->getNumero() * a;
						break;
					case '/':
//						p->desapilar();
						x=p->tope()->getNumero() / a;
						break;
					}
					p->desapilar(); //desapila el numero que estaba
				}
			}
	    p->apilar(new Token(x));
	    //cout << "\n\n ---- SALIENDO DE OPERAR ----- \n\n";
		};
		
	public:
		Calculadora(Pila *pila){
			pilaOriginal = pila;
		};
		
		int calcular(Pila *p){
			Token val= *pilaOriginal->tope();
			//----------VALORE TOKEN
			/*
			cout << "\nValor del Token: ";
			if(val.getType()=="number"){
				cout << pilaOriginal->tope()->getNumero();
			}else{
				cout << pilaOriginal->tope()->getChar();
			}
			//---------- EO VALORE TOKEN
	  	cout << "\nPilaOriginal Actual:" << pilaOriginal->toPrint("");
	  	cout << "\nPila P en tr�nsito: " << p->toPrint("");
	  	*/
			if(val.getType() == "number"){
				operar(p,val.getNumero());
				pilaOriginal->desapilar();
			}else if(val.getChar() == '+' || val.getChar() == '-' || val.getChar() == '*' || val.getChar() == '/'){
				pilaOriginal->desapilar();
				p->apilar(&val);
			}else if( val.getChar() == '('){
				cout << "\nParentesis Abre";
				pilaOriginal->desapilar(); //elimina el parentesis abierto
				int valorSiguiente = calcular(new Pila());
				cout << "\nValore siguiente: " << valorSiguiente;
				
				operar(p,valorSiguiente); // esto retorna un entero hay que hacer las operaciones necesarias
			}else if(val.getChar() == ')'){
				pilaOriginal->desapilar(); //elimina el parentesis cerrado
				/*
				int n = p->tope()->getNumero(); //guarda el valor tope de la pila actual
				p->desapilar(); //desapila el numero
				if(p->esvacia()){
					cout << "\nRetornando " << n <<endl;
					return n;
				}else{
					cout << "\nRetornando " << -n << endl;
					return -n;
				}*/
				return p->tope()->getNumero();
			}
			if(! pilaOriginal->pilavacia() ){
			  calcular(p);
      }else{
				return p->tope()->getNumero();
      }
      return p->tope()->getNumero();
		}
	bool isnum(char valor){
	//comprobacion numero
	bool isNum = false;
		if(!isalpha(valor) && isalnum(valor))
			isNum = true;
		return isNum;
	}
	bool analizar(string cad){
		// Analizar parentesis (cantidad y cerrado)
		bool correcto= true;
		Pila *parentesis = new Pila();
		for(int i = 0 ; i < cad.length() ; i++){
			if(cad.at(i)=='(')
				parentesis->apilar(new Token('('));
			else if(cad.at(i)==')')
				if(parentesis->pilavacia())
					correcto = false;
				else
					parentesis->desapilar();
		}
		if(!parentesis->pilavacia())
			correcto = false;
		
			
			
		//analizador de imperfecciones
		bool correcto1 = true;
		string valor = "inicio";
		if((!isnum(cad.at(0))
			valor = "num";
		else if(cad.at(0)=='(')
				valor = "(";
			else 
				correcto1 = false;
		
		for(int i = 1 ; i < cad.length() ; i++)
		{
			if(cad.at(i)=='(')
				if(valor != "signo" && valor != "(")
					correcto1 = false;
					else
						valor = "(";
			else if(cad.at(i)==')')
					if(valor != "num" && valor != ")")
						correcto1 = false;
						else
							valor = ")";
				else if(cad.at(i)=='+' || cad.at(i)=='-' || cad.at(i)=='*' || cad.at(i)=='/')
		}
	}
		
};
//////////////// FIN CALCULADORA //////////////////

int main(){
  string cad;
  cin >> cad;
  
  Pila *p = new Pila();
  Token *x;
  for (int i = 0 ; i < cad.length() ; i++){
    if(cad.at(i) == '0')
    	x = new Token(0);
    else if(cad.at(i) == '1')
    	x = new Token(1);
    else if(cad.at(i) == '2')
    	x = new Token(2);
    else if(cad.at(i) == '3')
    	x = new Token(3);
    else if(cad.at(i) == '4')
    	x = new Token(4);
    else if(cad.at(i) == '5')
    	x = new Token(5);
    else if(cad.at(i) == '6')
    	x = new Token(6);
    else if(cad.at(i) == '7')
    	x = new Token(7);
    else if(cad.at(i) == '8')
    	x = new Token(8);
    else if(cad.at(i) == '9')
    	x = new Token(9);
    else
    	x = new Token(cad.at(i));
    p->apilar(x);
  }
  
  Pila *n = new Pila();
  while(!p->pilavacia()){
    n->apilar(p->tope());
    p->desapilar();
  }
  Calculadora *c = new Calculadora(n);
  cout << "Resultado: " << c->calcular(new Pila());
}

