package main

import (
	"fmt"
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
)

type Candidate struct {
	ID    uint   `json:"id"`
	Name  string `json:"name"`
	Url   string `json:"url"`
	Votes int    `json:"votes"`
}

func GetAllCandidates(c *gin.Context) {
	var candidates []Candidate

	db.Find(&candidates)
	if len(candidates) <= 0 {
		c.JSON(http.StatusNotFound, gin.H{})
		return
	}
	c.JSON(http.StatusOK, candidates)
}

// TODO: Neuter this things functionality
func CreateCandidate(c *gin.Context) {
	c.JSON(http.StatusForbidden, gin.H{"error": "Cannot create candidates at this time"})
	return
	/*
		var candidate Candidate

		name, exists := c.GetPostForm("name")
		if !exists {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Did not specify name for candidate"})
			return
		}

		url, exists := c.GetPostForm("url")
		if !exists {
			c.JSON(http.StatusBadRequest, gin.H{"error": "Did not specify url for candidate"})
			return
		}

		db.Where("name = ?", name).First(&candidate)
		if candidate.ID != 0 {
			c.JSON(http.StatusConflict, gin.H{"error": "Candidate by that name already exists"})
			return
		}

		candidate = Candidate{Name: name, Url: url}
		db.Save(&candidate)
		c.JSON(http.StatusCreated, gin.H{})
	*/
}

func GetCandidate(c *gin.Context) {
	var candidate Candidate

	candID := c.Param("id")

	sqlStr := fmt.Sprintf("select id, name, url, votes from candidates where id = %s", candID)
	db.Raw(sqlStr).Scan(&candidate)
	if candidate.ID == 0 {
		c.JSON(http.StatusNotFound, gin.H{})
		return
	}

	c.JSON(http.StatusOK, gin.H{"error": "Under construction // TODO: return results from query"})
}

func UpdateCandidate(c *gin.Context) {
	var candidate Candidate

	candID, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.JSON(http.StatusConflict, gin.H{"error": "Did not provide valid id"})
		return
	}

	db.First(&candidate, candID)
	if candidate.ID == 0 {
		c.JSON(http.StatusNotFound, gin.H{"error": "Unable to find candidate"})
		return
	}

	candName := c.PostForm("name")
	db.Model(&candidate).Update("name", candName)

	c.JSON(http.StatusOK, gin.H{})
}

func VoteForCandidate(c *gin.Context) {
	var candidate Candidate
	var user *User

	contextUser, ok := c.Get("user")
	if !ok {
		c.JSON(http.StatusInternalServerError, gin.H{"error": "Internal Error"})
		return
	}
	user = contextUser.(*User)

	candID, err := strconv.Atoi(c.Param("id"))
	if err != nil {
		c.JSON(http.StatusConflict, gin.H{"error": "Did not provide valid id"})
		return
	}

	if user.CandidateID == uint(candID) {
		c.JSON(http.StatusOK, gin.H{})
		return
	}

	db.First(&candidate, candID)
	if candidate.ID == 0 {
		c.JSON(http.StatusNotFound, gin.H{"error": "Unable to find candidate"})
		return
	}

	if user.CandidateID != 0 {
		var votedCand Candidate
		db.Model(user).Related(&votedCand, "Candidate")
		votedCand.Votes--
		db.Save(&votedCand)
	}

	candidate.Votes++
	db.Save(&candidate)

	user.CandidateID = candidate.ID
	db.Save(user)

	c.JSON(http.StatusOK, gin.H{})
}
