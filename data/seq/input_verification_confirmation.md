sequenceDiagram
    User->>Frontend: 予約情報入力 (会議室ID, 開始日時, 終了日時)
    activate Frontend
    Frontend->>Frontend: 日付フォーマット検証 (fun-045)
    Frontend->>Frontend: 時刻フォーマット検証 (fun-046)
    Frontend->>Frontend: 日付期間妥当性チェック (fun-050)
    Frontend-->>User: (もしクライアント側でエラーがあれば) 入力エラー表示
    alt クライアント側検証成功
        Frontend->>Backend: 予約情報送信
        activate Backend
        Backend->>Backend: 会議室IDの存在チェック (fun-043)
        Backend->>DB: 会議室情報を照会
        activate DB
        DB-->>Backend: 会議室情報取得
        deactivate DB
        Backend-->>Backend: (もし会議室IDが存在しなければ) エラー処理
        Backend->>Backend: 日付フォーマット検証 (fun-045)
        Backend->>Backend: 時刻フォーマット検証 (fun-046)
        Backend->>Backend: 日付期間妥当性チェック (fun-050)
        Backend->>Backend: 予約時間重複チェック (fun-047)
        Backend->>DB: 既存予約を照会 (会議室ID, 期間)
        activate DB
        DB-->>Backend: 既存予約リスト取得
        deactivate DB
        Backend-->>Backend: (もし重複があれば) エラー処理
        Backend-->>Frontend: 検証結果を応答 (成功/エラー)
        deactivate Backend
        Frontend-->>User: 検証結果表示
    else クライアント側検証失敗
        Frontend-->>User: 入力エラー表示
    end
    deactivate Frontend