package main

import (
	"bytes"
	"fmt"
	"log"
	"net/http"
	"os"

	"github.com/gin-gonic/gin"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/sqlite"
)

var db *gorm.DB

func init() {
	var err error
	db, err = gorm.Open("sqlite3", "vote.db")
	if err != nil {
		log.Fatal(err)
	}

	db.AutoMigrate(&Candidate{})
	db.AutoMigrate(&User{})
	db.AutoMigrate(&SecretFlag{})

	var secretFlag SecretFlag
	db.First(&secretFlag, 1)
	if secretFlag.ID == 0 {
		secretFlag.Flag = "flag{uh_oh_you_sh0uldnt_be_seeing_this}"
		db.Save(&secretFlag)
	}

	var admin User
	db.Where("name = ?", "admin@admin.com").First(&admin)
	if admin.ID == 0 {
		admin.Name = "admin@admin.com"
		admin.IsAdmin = true
		admin.hashPassword("password")
		admin.createAPIToken()
		db.Save(&admin)
	}
}

type bodyLogWriter struct {
	gin.ResponseWriter
	body *bytes.Buffer
}

func (w bodyLogWriter) Write(b []byte) (int, error) {
	w.body.Write(b)
	return w.ResponseWriter.Write(b)
}

func ginBodyLogMiddleware(c *gin.Context) {
	blw := &bodyLogWriter{body: bytes.NewBufferString(""), ResponseWriter: c.Writer}
	c.Writer = blw
	c.Next()
	statusCode := c.Writer.Status()
	if statusCode >= 400 {
		fmt.Println("Response body: " + blw.body.String())
	}
}

func APIHeaderMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		apiAgent := c.GetHeader("X-API-AGENT")
		if apiAgent != "ANDROID" {
			c.JSON(http.StatusExpectationFailed, gin.H{})
			c.Abort()
		}
	}
}

func APIUserAuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		apiToken := c.GetHeader("X-API-TOKEN")
		if apiToken == "" {
			c.JSON(http.StatusExpectationFailed, gin.H{"error": "API token not set"})
			c.Abort()
		}

		user := FindUser(apiToken)
		if user == nil {
			c.JSON(http.StatusExpectationFailed, gin.H{"error": "Invalid user"})
			c.Abort()
		}
		c.Set("user", user)
	}
}

func APIAdminAuthMiddleware() gin.HandlerFunc {
	return func(c *gin.Context) {
		apiToken := c.GetHeader("X-API-TOKEN")
		if apiToken == "" {
			c.JSON(http.StatusExpectationFailed, gin.H{"error": "API token not set"})
			c.Abort()
		}

		user := FindUser(apiToken)
		if user == nil || !user.IsAdmin {
			c.JSON(http.StatusExpectationFailed, gin.H{"error": "Invalid user"})
			c.Abort()
		}
		c.Set("user", user)
	}
}

func main() {
	port := os.Getenv("PORT")

	if port == "" {
		log.Fatal("$PORT must be set")
	}
	defer db.Close()

	router := gin.New()
	router.Use(gin.Logger())

	router.Use(ginBodyLogMiddleware)
	router.Use(APIHeaderMiddleware())

	apiRouter := router.Group("/")
	{
		apiRouter.GET("/candidates", GetAllCandidates)
		apiRouter.GET("/candidates/:id", GetCandidate)
		apiRouter.POST("/user/login", LoginUser)
		apiRouter.POST("/user/register", RegisterUser)
	}

	authRouter := router.Group("/")
	authRouter.Use(APIUserAuthMiddleware())
	{
		authRouter.PUT("/vote/:id", VoteForCandidate)
	}

	adminRouter := router.Group("/")
	{
		adminRouter.POST("/candidates", CreateCandidate)
		adminRouter.GET("/code", GetAppCode)
	}

	addr := fmt.Sprintf(":%s", port)
	//router.RunTLS(addr, "./certs/server.pem", "./certs/server.key")
    router.Run(addr)
}
