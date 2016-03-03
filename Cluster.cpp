#include <algorithm>
	#include "Cluster.h"
	#include <sstream>
	
	//Included automatically by CLion
	#include <stdio.h>
	
	namespace Clustering {
	
	LNode::LNode(const Point &p, LNodePtr n) : point(p){
	next = n;
	}
	
	//default constructor 
	Cluster::Cluster(){
	__size = 0;
	__points = nullptr;
	}
	
	Cluster::Cluster(const Cluster &C1){
	__size = C1.getSize();
	__cpy(C1.__points);
	}
	
	Cluster& Cluster::operator=(const Cluster & other){
	if(this != &other){
	//delete & copy
	__size = other.__size;
	__del();
	__cpy(other.__points);
	}
	return *this;
	}
	Cluster::~Cluster(){
	__del();
	}
	
	int Cluster::getSize() const{
	return __size;
	}
	
	void Cluster::__del(){
	LNodePtr curr = __points;
	
	LNodePtr next;
	
	for(int i = 0; i < __size; ++i){
	if(__points != nullptr){
	next = curr->next;
	delete curr;
	curr = next;
	}
	}
	__points = nullptr;
	}
	
	void Cluster::__cpy(LNodePtr pts){
	if(pts != nullptr){
	LNodePtr null = nullptr;
	LNode *head = new LNode(pts->point,null);
	LNodePtr ptsCurr = pts->next;
	__points = head;
	LNodePtr curr = __points;
	
	while(ptsCurr != nullptr){
	curr->next = new LNode(ptsCurr->point,null);
	curr = curr->next;
	ptsCurr = ptsCurr->next;
	}
	}
	}
	
	bool Cluster::__in(const Point &p) const{
	
	}
	
	void Cluster::add(const Point & pNew) {
	
	if (__points == nullptr) {
	LNode *N = new LNode(pNew, nullptr); // new node created and pointing 
	__points = N;
	++__size;
	return;
	}
	
	if (__points->next == nullptr) {
	LNode *N = new LNode(pNew, nullptr);
	if(N->point < __points->point){ // attaches new val and assignes new ptr 
	N->next = __points;
	__points = N;
	++__size;
	return;
	} else {
	__points->next = N;
	++__size;
	return;
	}
	}
	
	LNode *N = new LNode(pNew, nullptr);
	LNodePtr temp = __points->next;
	LNodePtr prev = __points;
	
	if(N->point < __points->point){
	__points = N;
	N->next = prev;
	++__size;
	return;
	}
	
	if(N->point < temp->point){
	prev->next = N;
	N->next = temp;
	++__size;
	return;
	}
	
	while (temp->next != nullptr) {
	if(N->point < temp->point){
	prev = N;
	N->next = prev;
	++__size;
	return;
	}
	temp = temp->next;
	prev = prev->next;
	}
	temp->next = N;
	++__size;
	}
	
	
	const Point & Cluster::remove(const Point &P1) {
	// Check if the given point exists in the cluster.
	
	LNodePtr temp = __points;
	
	if (temp->point == P1) {
	
	temp = __points;
	if (__size > 0) {
	__points = __points->next;
	--__size;
	}
	delete temp;
	} else {
	int i = 0;
	LNodePtr prev = temp;
	temp = temp->next;
	
	for (; i < __size; ++i) {
	if (temp->point == P1) {
	if (temp->next == nullptr) {
	prev->next = nullptr;
	delete temp;
	__size--;
	} else {
	prev->next = temp->next;
	}
	}
	temp = temp->next;
	prev = prev->next;
	}
	}
	return P1;
	}
	
	bool Cluster::contains(const Point & P1){
	
	LNodePtr temp;
	if(P1 == __points->point){
	return true;
	}
	if(__points->next == nullptr){
	return false;
	}
	temp = __points->next;
	for(int i = 1; i < __size; ++i){
	if(temp->point == P1){
	return true;
	}
	temp = temp->next;
	}
	return false;
	}
	
	const Point &Cluster::operator[](unsigned int index) const{
	LNodePtr temp;
	temp = __points;
	
	if(index == 0){
	return __points->point;
	}
	
	for(int i = 0; i < index; ++i){
	temp = temp->next;
	}
	return temp->point;
	}
	
	bool operator==(const Cluster & C1, const Cluster &C2){
	if(C1.__size != C2.__size){
	return false;
	}
	
	for(int i = 0; i < C1.__size; ++i){
	if(C1[i] != C2[i]){
	return false;
	}
	}
	return true;
	}
	bool operator!=(const Cluster & C1, const Cluster & C2){
	return !(C1 == C2);
	}
	
	Cluster & Cluster::operator+=(const Point & P1){
	this->add(P1);
	return *this;
	}
	Cluster & Cluster::operator-=(const Point & P1){
	this->remove(P1);
	return *this;
	}
	
	// Members: Compound assignment (Cluster argument)
	Cluster &Cluster::operator+=(const Cluster & C2) { // union
	
	for(int i = 0; i < C2.getSize(); ++i){
	if(!(this->contains(C2[i]))){
	this->add(C2[i]);
	}
	}
	
	return *this;
	}
	
	Cluster &Cluster::operator-=(const Cluster & C2){ // (asymmetric) difference
	for(int i = 0; i < C2.getSize(); ++i){
	if(this->contains(C2[i])){
	this->remove(C2[i]);
	}
	}
	return *this;
	}
	// Friends: Arithmetic (Cluster and Point)
	const Cluster operator+(const Cluster &C1, const Point &P1){
	Cluster *C2 = new Cluster(C1);
	if(!(C2->contains(P1))){
	C2->add(P1);
	}else{
	return C1;
	}
	return *C2;
	}
	const Cluster operator-(const Cluster &C1, const Point &P1){
	Cluster *C2 = new Cluster(C1);
	if(C2->contains(P1)){
	C2->remove(P1);
	}else{
	return C1;
	}
	return *C2;
	}
	
	// Friends: Arithmetic (two Clusters)
	const Cluster operator+(const Cluster &C1, const Cluster &C2){
	Cluster *C3 = new Cluster(C1);
	Cluster *C4 = new Cluster(C2);
	
	for(int i = 0; i < C4->getSize(); ++i){
	if(!(C3->contains(C2[i]))){
	C3->add(C2[i]);
	}
	}
	delete C4;
	return *C3;
	}
	
	const Cluster operator-(const Cluster &C1, const Cluster &C2){
	Cluster *C3 = new Cluster(C1);
	Cluster *C4 = new Cluster(C2);
	
	for(int i = 0; i < C3->getSize(); ++i){
	if(C3->contains(C2[i])){
	C3->remove(C2[i]);
	}
	}
	delete C4;
	return *C3;
	}
	
	std::ostream &operator<<(std::ostream & out, const Cluster & C1){
	LNodePtr temp = C1.__points;
	for(int i = 0; i < C1.__size; ++i){
	out << temp->point << std::endl;
	temp = temp->next;
	}
	return out;
	}
	
	// Function is messy, and probably has a lot of unnecessary stuff, but works.
	std::istream &operator>>(std::istream & in, Cluster& C1) {
	
	std::string temp;
	std::getline(in, temp);
	std::stringstream s;
	s.str(temp);
	
	int dim = 0;
	double tempD = 0;
	while (!s.eof()) {
	s >> tempD;
	s.ignore(10, ',');
	dim++;
	}
	
	bool run = false;
	int index = 0;
	while (in.peek() != EOF) {
	Point *nPoint = new Clustering::Point(dim);
	if (run) {
	temp = "";
	std::getline(in, temp);
	temp.erase(remove_if(temp.begin(), temp.end(), isspace), temp.end());
	}
	s.str("");
	s.clear();
	s.str(temp);
	
	while ((s.peek() != '\n') || (s.peek() != '\r') || (s.peek() != EOF)) {
	s >> tempD;
	nPoint->setValue(index, tempD);
	if ((s.peek() == '\n') || (s.peek() == '\r') || (s.peek() == EOF)) {
	run = true;
	C1.add(*nPoint);
	index = 0;
	delete nPoint;
	break;
	} else {
	s.ignore();
	index++;
	}
	}
	}
	return in;
	}
	}

