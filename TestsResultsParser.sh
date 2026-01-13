#!/bin/bash

GITHUB_STEP_SUMMARY="TestsResults.md"

echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "" >> $GITHUB_STEP_SUMMARY
echo "## 🧪 Test Results Summary: $(date)<br/>" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY

echo "| Result | Test | Time (ms) | Errors |" >> $GITHUB_STEP_SUMMARY
echo "| :--- | :--- | :--- | :--- |" >> $GITHUB_STEP_SUMMARY

# Parse Google Test JSON format
jq -r '
.testsuites[] | 
.testsuite[] |
"| " +
if .status == "RUN" and .result == "COMPLETED" and (.failures // 0) == 0 then "✅ "
elif (.failures // 0) > 0 then "❌ "
else "⚠️ "
end +
" | " + .classname + "." + .name + " | " +
(if .time then (.time | gsub("s$"; "") | tonumber | (. * 1000) | tostring) else "0" end) + " | " +
(if (.failures // 0) > 0 then 
  if .failure then 
    " **Failure Message: " + (.failure.message // .failure // "N/A") + "**" 
  else 
    " **Test Failed**" 
  end 
else "" end)
+ " | "
' TestsResults.json | sed 's/_/\\_/g' >> $GITHUB_STEP_SUMMARY

# Count test results
PASSED=$(jq '[.testsuites[].testsuite[] | select(.status == "RUN" and .result == "COMPLETED" and (.failures // 0) == 0)] | length' TestsResults.json)
FAILED=$(jq '[.testsuites[].testsuite[] | select((.failures // 0) > 0)] | length' TestsResults.json)
SKIPPED=$(jq '[.testsuites[].testsuite[] | select(.status == "NOTRUN")] | length' TestsResults.json)

echo "" >> $GITHUB_STEP_SUMMARY
echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "" >> $GITHUB_STEP_SUMMARY
echo "## 📊 Test Results Summary:" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo " * ✅ Passed: $PASSED" >> $GITHUB_STEP_SUMMARY
echo " * ❌ Failed: $FAILED" >> $GITHUB_STEP_SUMMARY
echo " * ⚠️ Skipped: $SKIPPED" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "-----------" >> $GITHUB_STEP_SUMMARY

if [[ "$FAILED" -gt 0 ]]; then
    echo "::error title=Test Failures::❌ $FAILED test(s) failed. See the test report for details."
    exit 1
fi
