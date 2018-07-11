package main

import (
	"net/http"

	"github.com/gin-gonic/gin"
	uuid "github.com/satori/go.uuid"
	"golang.org/x/crypto/bcrypt"
)

type User struct {
	ID          uint
	Name        string
	Password    string
	APIToken    string
	Candidate   Candidate
	CandidateID uint
	IsAdmin     bool
}

type SentUser struct {
	Name     string
	Password string
}

func (u *User) createAPIToken() {
	u1 := uuid.Must(uuid.NewV4())
	u.APIToken = u1.String()
}

func (u *User) hashPassword(password string) error {
	bytes, err := bcrypt.GenerateFromPassword([]byte(password), 14)
	if err != nil {
		return err
	}
	u.Password = string(bytes)
	return nil
}

func (u *User) checkPassword(password string) bool {
	err := bcrypt.CompareHashAndPassword([]byte(u.Password), []byte(password))
	return err == nil
}

func FindUser(apiToken string) *User {
	var user User

	db.Where("api_token = ?", apiToken).First(&user)
	if user.ID == 0 {
		return nil
	}
	return &user
}

func LoginUser(c *gin.Context) {
	var user User
	var sentUser SentUser

	if c.BindJSON(&sentUser) != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Please send correct user information"})
		return
	}

	db.Where("name = ?", sentUser.Name).First(&user)
	if user.ID == 0 {
		c.JSON(http.StatusConflict, gin.H{"error": "Cannot find user with that name"})
		return
	}

	if user.checkPassword(sentUser.Password) {
		c.JSON(http.StatusOK, gin.H{"admin": user.IsAdmin, "token": user.APIToken})
		return
	}

	c.JSON(http.StatusBadRequest, gin.H{"error": "Invalid password for user"})
}

func RegisterUser(c *gin.Context) {
	var user User
	var sentUser SentUser

	if c.BindJSON(&sentUser) != nil {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Please send correct user information"})
	}

	db.Where("name = ?", sentUser.Name).First(&user)
	if user.ID != 0 {
		c.JSON(http.StatusConflict, gin.H{"error": "User by that name already exists"})
		return
	}

	user = User{Name: sentUser.Name}
	user.hashPassword(sentUser.Password)
	user.createAPIToken()

	db.Save(&user)
	c.JSON(http.StatusCreated, gin.H{"token": user.APIToken})
}
