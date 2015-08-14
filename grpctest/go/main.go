package main

import (
	pb "../proto"
	context "golang.org/x/net/context"
	grpc "google.golang.org/grpc"
	"html/template"
	"io/ioutil"
	"log"
	"net/http"
	"os"
)

//GreeterClient part
type GreeterClient interface {
	// Sends a greeting
	SendSentence(ctx context.Context, in *pb.NluRequest, opts ...grpc.CallOption) (*pb.NluReply, error)
}

type greeterClient struct {
	cc *grpc.ClientConn
}

func NewGreeterClient(cc *grpc.ClientConn) GreeterClient {
	return &greeterClient{cc}
}

func (c *greeterClient) SendSentence(ctx context.Context, in *pb.NluRequest, opts ...grpc.CallOption) (*pb.NluReply, error) {
	out := new(pb.NluReply)
	err := grpc.Invoke(ctx, "/nlu_grpc.Greeter/SendSentence", in, out, c.cc, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

//web server part
type Page struct {
	Title string
	Body  []byte
}

func loadPage(title string) (*Page, error) {
	filename := title + ".txt"
	body, err := ioutil.ReadFile(filename)
	if err != nil {
		return nil, err
	}
	return &Page{Title: title, Body: body}, nil
}

func renderTemplate(w http.ResponseWriter, tmpl string, p *Page) {
	t, _ := template.ParseFiles(tmpl + ".html")
	t.Execute(w, p)
}

func viewHandler(w http.ResponseWriter, r *http.Request) {
	title := r.URL.Path[len("/view/"):]
	p, _ := loadPage(title)
	renderTemplate(w, "view", p)
}

func editHandler(w http.ResponseWriter, r1 *http.Request) {
	title := r1.URL.Path[len("/edit/"):]
	p, err1 := loadPage(title)
	if err1 != nil {
		p = &Page{Title: title}
	}
	renderTemplate(w, "edit", p)
}

func saveHandler(w http.ResponseWriter, r1 *http.Request) {
	title := r1.URL.Path[len("/save/"):]
	body := r1.FormValue("body")

	//grpc begin
	conn, err := grpc.Dial(address)
	if err != nil {
		log.Fatalf("did not connect: %v", err)
	}
	defer conn.Close()
	c := NewGreeterClient(conn)
	name := string(body)
	if len(os.Args) > 1 {
		name = os.Args[1]
	}
	r, err := c.SendSentence(context.Background(), &pb.NluRequest{Name: name})
	if err != nil {
		log.Fatalf("could not greet: %v", err)
	}
	log.Printf("Greeting: %s", r.Message)
	//grpc end

	p := &Page{Title: title, Body: []byte(r.Message)}
	renderTemplate(w, "save", p)
}

func showHandler(w http.ResponseWriter, r1 *http.Request) {
	title := r1.URL.Path[len("/show/"):]
	http.Redirect(w, r1, "/view/"+title, http.StatusFound)
}

const (
	address = "localhost:50051"
)

func main() {
	http.HandleFunc("/view/", viewHandler)
	http.HandleFunc("/edit/", editHandler)
	http.HandleFunc("/save/", saveHandler)
	http.HandleFunc("/show/", showHandler)
	http.ListenAndServe(":8080", nil)

}
