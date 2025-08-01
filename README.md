# Models 
Ollama modelを見る
curl http://172.23.203.28:11434/api/tags | ConvertFrom-Json | Select-Object -ExpandProperty models | Format-Table name, model, size
