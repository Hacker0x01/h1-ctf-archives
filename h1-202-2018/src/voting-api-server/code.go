package main

import (
	"net/http"

	"path/filepath"

	"github.com/gin-gonic/gin"
)

const BUILT_APPS = "built_apps"

func GetAppCode(c *gin.Context) {
	app, ok := c.GetQuery("app")
	if !ok {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Did not provide app query param"})
		return
	}

	if app == "client" || app == "server" {
		c.Header("Content-Description", "File Transfer")
		c.Header("Content-Transfer-Encoding", "binary")
		c.Header("Content-Type", "application/octet-stream")
		c.Header("Content-Disposition", "attachment; filename="+app)
		c.File(filepath.Join(BUILT_APPS, app))
	} else {
		c.JSON(http.StatusBadRequest, gin.H{"error": "Could not find application"})
		return
	}
}
