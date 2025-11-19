sequenceDiagram
    participant 管理者
    participant scr-011 会議室管理画面
    participant 会議室登録画面
    participant 会議室編集画面
    participant 確認ダイアログ
    participant bac-004 会議室追加
    participant bac-005 会議室検索表示
    participant bac-006 会議室削除
    participant bac 会議室情報更新
    participant DB 会議室テーブル

    管理者->>scr-011 会議室管理画面: 会議室管理画面表示リクエスト
    activate scr-011 会議室管理画面
    scr-011 会議室管理画面->>bac-005 会議室検索表示: 全会議室情報取得リクエスト (GET /api/rooms)
    activate bac-005 会議室検索表示
    bac-005 会議室検索表示->>DB 会議室テーブル: SELECT * FROM 会議室
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac-005 会議室検索表示: 会議室データ
    deactivate DB 会議室テーブル
    bac-005 会議室検索表示-->>scr-011 会議室管理画面: 会議室リスト
    deactivate bac-005 会議室検索表示
    scr-011 会議室管理画面-->>管理者: 会議室管理メニュー表示 (fun-010)
全会議室表示 (fun-013)
    deactivate scr-011 会議室管理画面

    管理者->>scr-011 会議室管理画面: 新規会議室追加ボタンクリック
    activate scr-011 会議室管理画面
    scr-011 会議室管理画面->>会議室登録画面: 会議室登録画面へ遷移
    deactivate scr-011 会議室管理画面
    activate 会議室登録画面
    会議室登録画面-->>管理者: 会議室情報入力フォーム表示 (fun-015, fun-016, fun-017)
    管理者->>会議室登録画面: 会議室ID, 会議室名, 収容人数を入力
    会議室登録画面->>bac-004 会議室追加: 会議室追加リクエスト (POST /api/rooms)
    activate bac-004 会議室追加
    bac-004 会議室追加->>DB 会議室テーブル: INSERT INTO 会議室 (id, 部屋名, 収容人数) VALUES (...)
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac-004 会議室追加: 登録結果
    deactivate DB 会議室テーブル
    bac-004 会議室追加-->>会議室登録画面: 「会議室追加成功」またはエラー (fun-011)
    deactivate bac-004 会議室追加
    会議室登録画面-->>scr-011 会議室管理画面: 会議室管理画面へ戻る
    deactivate 会議室登録画面

    管理者->>scr-011 会議室管理画面: 検索キーワード入力
「検索」ボタンクリック
    activate scr-011 会議室管理画面
    scr-011 会議室管理画面->>bac-005 会議室検索表示: 会議室検索リクエスト (GET /api/rooms?keyword=...)
    activate bac-005 会議室検索表示
    bac-005 会議室検索表示->>DB 会議室テーブル: SELECT * FROM 会議室 WHERE 部屋名 LIKE '%keyword%' OR id LIKE '%keyword%'
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac-005 会議室検索表示: 検索結果
    deactivate DB 会議室テーブル
    bac-005 会議室検索表示-->>scr-011 会議室管理画面: 検索結果リスト
    deactivate bac-005 会議室検索表示
    scr-011 会議室管理画面-->>管理者: 検索結果表示 (fun-012)
    deactivate scr-011 会議室管理画面

    管理者->>scr-011 会議室管理画面: 対象会議室の「編集」ボタンクリック
    activate scr-011 会議室管理画面
    scr-011 会議室管理画面->>会議室編集画面: 会議室編集画面へ遷移 (会議室ID)
    deactivate scr-011 会議室管理画面
    activate 会議室編集画面
    会議室編集画面->>bac-005 会議室検索表示: 会議室詳細情報取得リクエスト (GET /api/rooms/{id})
    activate bac-005 会議室検索表示
    bac-005 会議室検索表示->>DB 会議室テーブル: SELECT * FROM 会議室 WHERE id = {id}
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac-005 会議室検索表示: 会議室詳細データ
    deactivate DB 会議室テーブル
    bac-005 会議室検索表示-->>会議室編集画面: 編集フォームに既存情報を表示 (fun-015, fun-016, fun-017)
    deactivate bac-005 会議室検索表示
    管理者->>会議室編集画面: 会議室名, 収容人数を変更
「更新」ボタンクリック
    会議室編集画面->>bac 会議室情報更新: 会議室情報更新リクエスト (PATCH /api/rooms/{id})
    activate bac 会議室情報更新
    bac 会議室情報更新->>DB 会議室テーブル: UPDATE 会議室 SET 部屋名 = ..., 収容人数 = ... WHERE id = {id}
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac 会議室情報更新: 更新結果
    deactivate DB 会議室テーブル
    bac 会議室情報更新-->>会議室編集画面: 「会議室情報更新成功」またはエラー (fun-018)
    deactivate bac 会議室情報更新
    会議室編集画面-->>scr-011 会議室管理画面: 会議室管理画面へ戻る
    deactivate 会議室編集画面

    管理者->>scr-011 会議室管理画面: 対象会議室の「削除」ボタンクリック (会議室ID)
    activate scr-011 会議室管理画面
    scr-011 会議室管理画面->>確認ダイアログ: 削除確認表示
    activate 確認ダイアログ
    確認ダイアログ-->>管理者: 削除してよろしいですか？
    管理者->>確認ダイアログ: 「はい」を選択
    確認ダイアログ->>bac-006 会議室削除: 会議室削除リクエスト (DELETE /api/rooms/{id})
    deactivate 確認ダイアログ
    activate bac-006 会議室削除
    bac-006 会議室削除->>DB 会議室テーブル: DELETE FROM 会議室 WHERE id = {id}
    activate DB 会議室テーブル
    DB 会議室テーブル-->>bac-006 会議室削除: 削除結果
    deactivate DB 会議室テーブル
    bac-006 会議室削除-->>scr-011 会議室管理画面: 「会議室削除成功」またはエラー (fun-014)
    deactivate bac-006 会議室削除
    scr-011 会議室管理画面-->>scr-011 会議室管理画面: 会議室リスト再表示 (fun-013)
    deactivate scr-011 会議室管理画面