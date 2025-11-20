sequenceDiagram
    participant システム利用者
    participant フロントエンド
    participant バックエンド
    participant データベース

    システム利用者->>フロントエンド: ホーム画面表示 (scr-004)
    activate フロントエンド
    フロントエンド-->>システム利用者: ホーム画面を表示
    deactivate フロントエンド

    システム利用者->>フロントエンド: 予約管理メニュー選択 (fun-019)
    activate フロントエンド
    フロントエンド-->>システム利用者: 予約管理メニュー選択肢の表示
(例: 1.予約追加, 2.予約検索, 3.全予約表示, ...)
    deactivate フロントエンド

    システム利用者->>フロントエンド: 予約追加を選択
    activate フロントエンド
    フロントエンド-->>システム利用者: 予約作成画面表示 (scr-007)
(予約ID, ユーザーID, 会議室ID, 開始日時, 終了日時入力プロンプト)
    システム利用者->>フロントエンド: 予約情報を入力
    フロントエンド->>バックエンド: 予約追加リクエスト (POST /api/reservations)
bac-007 予約追加
(roomId, userId, startTime, endTime)
    activate バックエンド
    バックエンド->>データベース: ユーザーID存在確認 (SELECT FROM ユーザー WHERE id = userId)
    activate データベース
    データベース-->>バックエンド: ユーザー情報返却
    deactivate データベース
    バックエンド->>データベース: 会議室ID存在確認 (SELECT FROM 会議室 WHERE id = roomId)
    activate データベース
    データベース-->>バックエンド: 会議室情報返却
    deactivate データベース
    バックエンド->>データベース: 予約重複確認 (SELECT FROM 予約 WHERE 会議室ID = roomId AND ...時間重複チェック)
    activate データベース
    データベース-->>バックエンド: 既存予約情報または空き情報返却
    deactivate データベース
    alt 予約可能
        バックエンド->>データベース: 新規予約情報登録 (INSERT INTO 予約 VALUES (...))
        activate データベース
        データベース-->>バックエンド: 予約登録成功
        deactivate データベース
        バックエンド-->>フロントエンド: 予約追加成功メッセージ (201 Created)
    else 予約不可
        バックエンド-->>フロントエンド: エラーメッセージ (400 Bad Request)
(例: ユーザーIDが存在しません, 予約が重複しています)
    end
    deactivate バックエンド
    フロントエンド-->>システム利用者: 「予約追加成功」メッセージ
またはエラーメッセージ
    フロントエンド->>システム利用者: マイ予約一覧画面表示 (scr-008) または予約詳細画面表示 (scr-009)
    deactivate フロントエンド

    システム利用者->>フロントエンド: 全予約表示を選択
    activate フロントエンド
    フロントエンド->>バックエンド: 全予約取得リクエスト (GET /api/reservations)
bac-008 予約検索と表示 日付ソート
    activate バックエンド
    バックエンド->>データベース: 全予約情報取得 (SELECT * FROM 予約 ORDER BY 開始日時)
    activate データベース
    データベース-->>バックエンド: 全予約情報リスト返却
    deactivate データベース
    バックエンド-->>フロントエンド: 全予約情報リスト (200 OK)
    deactivate バックエンド
    フロントエンド-->>システム利用者: 全予約情報リスト表示
(scr-008 マイ予約一覧画面)
    deactivate フロントエンド