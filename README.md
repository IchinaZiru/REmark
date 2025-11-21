# Models 
Ollama modelを見る
curl http://172.23.203.28:11434/api/tags | ConvertFrom-Json | Select-Object -ExpandProperty models | Format-Table name, model, size

python scripts/MCQT/run_mcqt_onlyfunc_maxfuncs.py ` 
--model gpt-4o`                                                                                          
--pool_root data/mcqt/choices/guide `                                                                     
--correct_root data/mcqt/correct_choices `
--levels 0,7 `                                                                                            
--code_pattern "data/levels/L{level}/2048_L{level}.c" `
--out_root outputs/mcqt_results/gpt-4o `
--start_func "main" `
--max_funcs 5 `                                                                                           
--func_pick head `
--dump_prompts data/debug/prompts