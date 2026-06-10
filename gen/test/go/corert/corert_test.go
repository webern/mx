package corert_test

import (
	"os"
	"path/filepath"
	"testing"

	"github.com/webern/mx/gen/test/go/corert"
)

func repoRoot(t *testing.T) string {
	t.Helper()
	root := os.Getenv("MX_REPO_ROOT")
	if root == "" {
		// Walk upward from the test file to find the repo root (contains data/).
		dir, err := os.Getwd()
		if err != nil {
			t.Fatal(err)
		}
		for {
			if _, err := os.Stat(filepath.Join(dir, "data")); err == nil {
				return dir
			}
			parent := filepath.Dir(dir)
			if parent == dir {
				t.Fatal("cannot find repo root (directory containing data/)")
			}
			dir = parent
		}
	}
	return root
}

func TestCoreRoundtrip(t *testing.T) {
	root := repoRoot(t)
	dataRoot := filepath.Join(root, "data")
	files, err := corert.DiscoverInputFiles(dataRoot)
	if err != nil {
		t.Fatalf("discover input files: %v", err)
	}
	if len(files) == 0 {
		t.Fatal("no input files discovered")
	}

	for _, absPath := range files {
		name := corert.ToTestName(absPath, dataRoot)
		t.Run(name, func(t *testing.T) {
			result := corert.RunCoreRoundtrip(absPath)
			if result.Skipped {
				t.Skip(result.Message)
			}
			if !result.OK {
				if result.ExpectedXML != "" || result.ActualXML != "" {
					corert.WriteFailureFiles(root, name, result.ExpectedXML, result.ActualXML)
				}
				t.Errorf("core roundtrip failed: %s", result.Message)
			}
		})
	}
}
