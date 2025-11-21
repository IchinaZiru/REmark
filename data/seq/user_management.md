sequenceDiagram
    participant システム利用者 as User
    participant ユーザー管理画面 as Frontend
    participant ユーザー管理API as Backend
    participant データベース as DB

    User->>Frontend: ユーザー管理メニューを表示
    Frontend->>Backend: 全ユーザー情報を取得 (GET /api/users)
    Backend->>DB: 全ユーザー情報を検索
    DB-->>Backend: ユーザー情報リスト
    Backend-->>Frontend: ユーザー情報リスト
    Frontend->>User: ユーザー管理メニューと全ユーザーを表示

    User->>Frontend: 新規ユーザー追加を選択
    Frontend->>User: ユーザーIDとユーザー名の入力要求
    User->>Frontend: ユーザーIDとユーザー名を入力
    Frontend->>Backend: ユーザー追加リクエスト (POST /api/users)
    Backend->>DB: 新しいユーザー情報を保存
    DB-->>Backend: 保存結果
    Backend-->>Frontend: ユーザー追加成功/エラー
    Frontend->>User: ユーザー追加結果を表示

    User->>Frontend: ユーザー情報更新を選択
    Frontend->>User: 更新対象ユーザーIDと新しいユーザー名の入力要求
    User->>Frontend: ユーザーIDと新しいユーザー名を入力
    Frontend->>Backend: ユーザー情報更新リクエスト (PUT /api/users/{id})
    Backend->>DB: ユーザー情報を更新
    DB-->>Backend: 更新結果
    Backend-->>Frontend: ユーザー情報更新成功/エラー
    Frontend->>User: ユーザー情報更新結果を表示

    User->>Frontend: ユーザー削除を選択
    Frontend->>User: 削除対象ユーザーIDの入力要求と確認
    User->>Frontend: ユーザーIDを入力し削除を確定
    Frontend->>Backend: ユーザー削除リクエスト (DELETE /api/users/{id})
    Backend->>DB: ユーザー情報を削除
    DB-->>Backend: 削除結果
    Backend-->>Frontend: ユーザー削除成功/エラー
    Frontend->>User: ユーザー削除結果を表示

    User->>Frontend: ユーザー検索キーワードを入力
    Frontend->>Backend: ユーザー検索リクエスト (GET /api/users?keyword=XXX)
    Backend->>DB: 検索キーワードでユーザーを検索
    DB-->>Backend: 検索結果のユーザー情報リスト
    Backend-->>Frontend: 検索結果のユーザー情報リスト
    Frontend->>User: 検索結果を表示