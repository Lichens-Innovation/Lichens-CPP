GITHUB_STEP_SUMMARY="TestsResults.md"

echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "" >> $GITHUB_STEP_SUMMARY
echo "## 🧪 Test Results Summary: `date`<br/>" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY

echo "| Result | Test | Time (ms) | Errors |" >> $GITHUB_STEP_SUMMARY
echo "| :--- | :--- | :--- | :--- |" >> $GITHUB_STEP_SUMMARY
jq -r '
.report.tests[] | 
"| " +
if .outcome == "failed" then "❌ "
elif .outcome == "passed" then "✅ "
elif .outcome == "skipped" then "⚠️ "
else "? " end +
" | " + .name + " | " +
(.duration | tonumber | (1000000 * . | round / 1000) | tostring) + " | " +
if .outcome == "failed" then " **Failure Message: " + (.call.longrepr // "N/A") else "" end
+ " | "
' TestsResults.json | sed 's/_/\\_/g' >> $GITHUB_STEP_SUMMARY

PASSED=$(jq '[.report.tests[] | select(.outcome=="passed")] | length' TestsResults.json)
FAILED=$(jq '[.report.tests[] | select(.outcome=="failed")] | length' TestsResults.json)
SKIPPED=$(jq '[.report.tests[] | select(.outcome=="skipped")] | length' TestsResults.json)

echo "" >> $GITHUB_STEP_SUMMARY
echo "-----------" >> $GITHUB_STEP_SUMMARY 
echo "" >> $GITHUB_STEP_SUMMARY
echo "## 📊 Test Results Summary:" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo " * ✅ Passed:** $PASSED" >> $GITHUB_STEP_SUMMARY
echo " * ❌ Failed:** $FAILED" >> $GITHUB_STEP_SUMMARY
echo " * ⚠️ Skipped:** $SKIPPED" >> $GITHUB_STEP_SUMMARY
echo "" >> $GITHUB_STEP_SUMMARY
echo "-----------" >> $GITHUB_STEP_SUMMARY

if [[ "$FAILED" -gt 0 ]]; then
echo "::error title=Test Failures::❌ $FAILED test(s) failed. See the test report for details."
exit 1
fi