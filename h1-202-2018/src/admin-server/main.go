package main

import (
	"bytes"
	"fmt"
	"html/template"
	"log"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
)

var flag1 string
var flag2 string

// TODO: Strip binary
func getFlag() string {
	return flag2
}

type Candidate struct {
	Name string
	Url  string
}

// TODO: Require admin token to access
func GetAdminPage(c *gin.Context) {
	c.Header("Access-Control-Allow-Origin", "*")

	funcMap := template.FuncMap{
		"___________": getFlag,
	}

	templateText, ok := c.GetQuery("t")
	if !ok {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Did not provide t query param"})
		return
	}

	candName, ok := c.GetQuery("name")
	if !ok {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Did not provide name query param"})
		return
	}

	candImg, ok := c.GetQuery("image")
	if !ok {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Did not provide image query param"})
		return
	}

	client := &http.Client{}
	req, err := http.NewRequest("GET", candImg, nil)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Looks like we can't get that image, try another one?"})
		return
	}
	req.Header.Set("FLAG", flag1)
	req.Header.Set("CHALLENGE", "Calling out Foul Play")

	_, err = client.Do(req)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Looks like we can't get that image, try another one?"})
		return
	}

	tmpl, err := template.New("adminPage").Funcs(funcMap).Parse(templateText)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Oops! Try again :)"})
		return
	}

	cand := &Candidate{
		Name: candName,
		Url:  candImg,
	}

	var tpl bytes.Buffer
	err = tmpl.Execute(&tpl, cand)
	if err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Oops! Try again :)"})
		return
	}
	c.Header("Content-Type", "text/html")
	c.Data(200, "text/html", tpl.Bytes())
}

func main() {
	router := gin.New()
	router.Use(gin.Logger())

	port := os.Getenv("PORT")

	if port == "" {
		log.Fatal("$PORT must be set")
	}

	flag1 = os.Getenv("FLAG1")
	flag2 = os.Getenv("FLAG2")

	if flag1 == "" || flag2 == "" {
		log.Fatal("$FLAG1 and $FLAG2 must be set")
	}

	v1 := router.Group("/", gin.BasicAuth(gin.Accounts{
		"electionAdmin": "pickles",
	}))

	{
		v1.GET("/admin", GetAdminPage)
	}

	addr := fmt.Sprintf(":%s", port)
	router.Run(addr)
}
